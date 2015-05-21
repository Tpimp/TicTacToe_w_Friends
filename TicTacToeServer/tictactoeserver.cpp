#include "tictactoeserver.h"
#include "sys/types.h"
#include <sys/socket.h>
#include "netdb.h"
#include "netinet/in.h"
#include <sys/param.h>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "lobbypollthread.h"
#include "tictactoegamethread.h"
#include "unistd.h"
TicTacToeServer::TicTacToeServer(QObject *parent)
    : QObject(parent), mListeningThread(nullptr)
{
    mLobbyThread = new LobbyPollThread();
    connect(mLobbyThread, &LobbyPollThread::clientsReadyForGame,
                       this, &TicTacToeServer::createNewGame);
    connect(mLobbyThread, &LobbyPollThread::clientDisconnected,
                       this, &TicTacToeServer::clientDisconnected);

}


// Spins up the listening thread
void TicTacToeServer::startListening()
{
    mListeningThread = new ListeningThread();
    if(mListeningThread)
    {
        mLobbyThread->start();
        connect(mListeningThread, &ListeningThread::startedListening,
                           this, &TicTacToeServer::beganListening);
        connect(mListeningThread, &ListeningThread::newClientConnected,
                         mLobbyThread, &LobbyPollThread::incommingConnection);
        mListeningThread->start();
    }
}

void TicTacToeServer::clientDisconnected(int client)
{
    if(client >0)
        close(client);
}



// Spins up a new thread for the game
void TicTacToeServer::createNewGame(QString player1, int player1sd, QString player2, int player2sd)
{
    // create a new game thread and add them GAME LOBBIES.
    TicTacToeGameThread * new_thread( new TicTacToeGameThread(player1,player1sd,player2,player2sd));
    if(new_thread)
    {
        connect(new_thread, &TicTacToeGameThread::gameEnded,
                            this, &TicTacToeServer::gameEnded);
        mGameLobbies.append(new_thread);
        new_thread->start();
    }

}

// received game over, move players back into the lobby
void TicTacToeServer::gameEnded(TicTacToeGameThread *game)
{
    int index(mGameLobbies.indexOf(game));
    ConnectedClient playerX;
    ConnectedClient playerO;
    if(index >= 0)
    {
        // remove thread from GameLobby list
        TicTacToeGameThread * thread = mGameLobbies.takeAt(index);
        // fill local structs with  Connected Client info
        playerX.mSocketDescriptor = thread->mPlayerXSd;
        playerO.mSocketDescriptor = thread->mPlayerOSd;
        playerX.mState = IN_LOBBY;
        playerO.mState = IN_LOBBY;
        playerX.mName = thread->mPlayerX;
        playerO.mName = thread->mPlayerO;
        // Now notfiy everyone else of both clients coming into the lobby
        mLobbyThread->notifyOthersOfJoining(playerX);
        mLobbyThread->notifyOthersOfJoining(playerO);
        mLobbyThread->addClient(playerX.mName,playerX.mSocketDescriptor);
        mLobbyThread->addClient(playerO.mName,playerO.mSocketDescriptor);
        mLobbyThread->sendLobbyMembers(playerX);
        mLobbyThread->sendLobbyMembers(playerO);
    }
    disconnect(game,nullptr,this,nullptr); // clean up signals and slots
    delete game; // delete the game thread.
}


void TicTacToeServer::beganListening(int socket_desc)
{
    mServerSocket = socket_desc;
}


void TicTacToeServer::shutdownServer()
{
    qDebug() <<"Shuting down the Tic Tac Toe Server";
    if(mLobbyThread)
        qDebug() << mLobbyThread->lobbyCount() << " Clients will close";

    // clean up the threads
    if(mListeningThread)
    {
        mListeningThread->stopListening();
        mListeningThread->wait(2000);
        mListeningThread->terminate();
        mListeningThread->exit(0);
        QCoreApplication::processEvents();
    }
    // lobby thread will handle disconnecting to each client
    if(mLobbyThread)
    {
        mLobbyThread->stopPollingLobby();
        mLobbyThread->wait(2000);
        mListeningThread->terminate();
        mLobbyThread->exit(0);
        QCoreApplication::processEvents();
    }
    // if listening on the port
    if(mServerSocket > 0)
    {
        // shut it down and close it up
        shutdown(mServerSocket,0);
        close(mServerSocket);
    }

    delete mListeningThread;
    delete mLobbyThread;
    mListeningThread = nullptr;
    mLobbyThread = nullptr;
    while(mGameLobbies.length() > 0)
    {
        TicTacToeGameThread* thread(mGameLobbies.takeFirst());
        thread->terminate(); // kill all the game threads (forcefully)
        delete thread;
    }
}


TicTacToeServer::~TicTacToeServer()
{

    if(mListeningThread)
    {
        mListeningThread->stopListening();
        mListeningThread->exit(0);
        QCoreApplication::processEvents();
    }
    if(mLobbyThread)
    {
        mLobbyThread->stopPollingLobby();
        mLobbyThread->exit(0);
        QCoreApplication::processEvents();
    }
    if(mServerSocket > 0)
    {
        shutdown(mServerSocket,0);
        close(mServerSocket);
    }

    delete mListeningThread;
    delete mLobbyThread;
    mListeningThread = nullptr;
    mLobbyThread = nullptr;
    while(mGameLobbies.length() > 0)
    {
        TicTacToeGameThread* thread(mGameLobbies.takeFirst());
        thread->terminate();
        delete thread;
    }
}

