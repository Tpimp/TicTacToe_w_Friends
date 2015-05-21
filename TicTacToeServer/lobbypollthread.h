#ifndef LOBBYPOLLTHREAD_H
#define LOBBYPOLLTHREAD_H

#include "tictactoeserver.h"
#include <QObject>
#include <QThread>
#include "sys/socket.h"
#include "sys/types.h"
#include "sys/time.h"
#include <QDebug>
#include "netdb.h"
#include "poll.h"
#include <netinet/in.h>
#include "sys/ioctl.h"


enum CLIENT_FUNCTION
{
    CLIENT_CONNECT,
    LOBBY_LIST,
    LEFT_LOBBY,
    LEFT_GAME,
    CLIENT_DISCONNECTED,
    REQUEST_CHALLENGE,
    MAKE_MOVE,
    UPDATE_BOARD,
    GAME_START,
    CLOSING,
    ACCEPT_CHALLENGE,
    NAME_CHANGE,
    MESSAGE,
};

/***********************************************
 * POLLs the connected sockets (in lobby) and
 *  receives then parses each lobby message.
 * If lobby message is AcceptRequest, begins  to
 * spin up a new game and passes off to
 * TicTacToeGameThread
 *
 * *********************************************/

class LobbyPollThread : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE {
        char read_buffer[1200];
        QByteArray current_message;
        ssize_t  bytes_read(0);
        int count(-1);
        mRunning  = true;
        int clientsToRecv;
        mField =nullptr; // mField is altered in AddClient and RemoveClient
        while(mRunning)
        {
            if(mField) // if anybody in the lobby
            {
               clientsToRecv = poll(mField,mFieldSize,10);
               for(int i(0); i < mClientsInLobby.length(); ++i)
               {
                   ConnectedClient &  current_client = mClientsInLobby[i];
                   if(mField[i].revents == POLLIN)
                   {
                       // check each socket for bytes ready for read
                        ioctl(current_client.mSocketDescriptor, FIONREAD, &count);
                       // check if this socket is needing serviced
                       if(count > 0)
                       {
                           current_message.clear();
                           bytes_read = 0;

                           while(count > 0) // poll for next message
                           {
                               // must be some data or read is 0, grab it
                               bytes_read = ::recv(current_client.mSocketDescriptor,read_buffer,1200,0);
                               // append it to the byte array
                               current_message.append(read_buffer,bytes_read);
                               QThread::usleep(800);//check anything left, or service it
                               ioctl(current_client.mSocketDescriptor, FIONREAD, &count);
                           }
                           // finished fetching message.
                           serviceLobbyMessage(current_client,current_message);
                       }
                    }
                   else if (mField[i].revents == POLLHUP)
                   {    // receivedhangup (probably wont reach here)
                       removeClient(current_client);
                       // Client send disconnect as a message
                       emit clientDisconnected(current_client.mSocketDescriptor);
                   }
               }
               msleep(45);
            }
            else // mClient is not even allocated wait a second for next connection
                msleep(1000);
        }
    }
public:
    LobbyPollThread();
    ~LobbyPollThread();
    int lobbyCount()
    {
        return mClientsInLobby.length();
    }

public slots:
    void addClient(QString name,int socket_descriptor);
    void removeClient(ConnectedClient client);
    void incommingConnection(int socket_desc);
    void notifyOthersOfJoining(ConnectedClient client);
    void notifyOthersOfLeaving(ConnectedClient  client);
    void stopPollingLobby();
    void sendLobbyMembers( ConnectedClient client);
    void sendChallenge(ConnectedClient from, ConnectedClient sender);
    void sendChallengeAccepted(ConnectedClient from, ConnectedClient to, bool accepted);
    void sendNameChanged(QString name, ConnectedClient to);

signals:
    void playersMatched(ConnectedClient  playerX, ConnectedClient playerO);
    void clientMessageReady(QString name, QString message);
    void networkMoveReady(QString name, int cell);
    void clientDisconnected(int client);
    void clientsReadyForGame(QString player1, int player1sd, QString player2, int player2sd);

private:
    void              serviceLobbyMessage(ConnectedClient &  sender, QByteArray & message);
    QList<ConnectedClient>      mClientsInLobby;
    bool                                             mRunning;
    QStringList                                mCurrentMesage;
    pollfd                                        *mField;
    int                                                mFieldSize;

};

#endif // LOBBYPOLLTHREAD_H
