#include "clientconnection.h"
#include <QDebug>
#include "sys/socket.h"
#include <netdb.h>
#include <netinet/in.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QThread>
#include <QJsonArray>

#define DEBUG_MODE

ClientConnection::ClientConnection(QObject * parent) :
    QObject(parent), mClientIoThread(nullptr)
{

}

void ClientConnection::connectToServer(QString name)
{
    struct sockaddr_in serv_addr;
    int socket_d(-1);
    #ifdef DEBUG_MODE
        socket_d = ::socket(AF_INET,SOCK_STREAM,0);
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family  = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        serv_addr.sin_port = htons(40582);
    #else  // will eventually setup for conection as Server IP Unx.cset.oit.edu
        socket_d = ::socket(AF_INET,SOCK_STREAM,0);
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family  = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(); // use  addrinfo to lookup ip
        serv_addr.sin_port = htons(40582);
    #endif
      // Connect to server must be called with :: to not conflict with QObject::connect
    if(::connect(socket_d, (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) >= 0)
    {
        //qDebug() << "Connected Succesfully!"
        // spin up thread to handle socket
        mClientIoThread = new ClientIOThread(name,socket_d);
        if(mClientIoThread)
        {
            // forward all signals through ClientConnection object
            connect(mClientIoThread, &ClientIOThread::clientFound,
                               this, &ClientConnection::clientFound);
            connect(mClientIoThread, &ClientIOThread::connectedToServer,
                               this, &ClientConnection::connectedToServer);
            connect(mClientIoThread, &ClientIOThread::clientLeftLobby,
                              this, &ClientConnection::clientLeftLobby);
            connect(mClientIoThread, &ClientIOThread::receivedChallenge,
                              this, &ClientConnection::receivedChallenge);
            connect(mClientIoThread, &ClientIOThread::receivedChallengeReply,
                              this, &ClientConnection::receivedChallengeReply);
            connect(mClientIoThread, &ClientIOThread::networkGameEnded,
                              this, &ClientConnection::networkGameEnded);
            connect(mClientIoThread, &ClientIOThread::moveMade,
                              this, &ClientConnection::moveMade);
            connect(mClientIoThread, &ClientIOThread::disconnectedFromServer,
                              this, &ClientConnection::disconnectedFromServer);
            connect(mClientIoThread, &ClientIOThread::clearLobby,
                              this, &ClientConnection::clearLobby);
            connect(mClientIoThread, &ClientIOThread::boardUpdated,
                              this, &ClientConnection::boardUpdated);
            connect(mClientIoThread, &ClientIOThread::networkGameStarted,
                              this, &ClientConnection::networkGameStarted);
            connect(mClientIoThread, &ClientIOThread::serverNameChanged,
                             this, &ClientConnection::serverNameChanged);
             mClientIoThread->start();
        }
    }
    else
    {
        qDebug() << "Error connecting to server";
    }
}

void ClientConnection::requestChallenge(QString opponent)
{
    if(mClientIoThread)
        mClientIoThread->requestChallenge(opponent);
}
void ClientConnection::sendMoveMade(bool isX, int cell)
{
    if(mClientIoThread)
        mClientIoThread->sendMoveMade(isX, cell);
}

void ClientConnection::sendAcceptChallenge(QString opponent, bool accepted)
{
    if(mClientIoThread)
        mClientIoThread->sendAcceptChallenge(opponent,accepted);
}
void ClientConnection::sendMessage(QString message)
{
    if(mClientIoThread)
        mClientIoThread->sendMessage(message);
}

void ClientConnection::disconnectFromServer()
{
    if(mClientIoThread)
        mClientIoThread->disconnectFromServer();
}

void ClientConnection::sendLeavingGame()
{
    if(mClientIoThread)
        mClientIoThread->sendLeavingGame();
}


void ClientConnection::sendLeavingLobby()
{
    if(mClientIoThread)
        mClientIoThread->sendLeavingLobby();
}

ClientConnection::~ClientConnection()
{
    // if the client thread is allocated
    if(mClientIoThread)
    {
        // send disconnect
        mClientIoThread->disconnectFromServer();
        // wait for disconnect
        mClientIoThread->wait(2000);
        // close out thread forcefully if not
        mClientIoThread->terminate();
        delete mClientIoThread;
    }
}

