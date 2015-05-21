#ifndef CLIENTIOTHREAD_H
#define CLIENTIOTHREAD_H

#include <QObject>
#include<QThread>
#include <QDebug>
#include "sys/socket.h"
#include <netdb.h>
#include <netinet/in.h>
#include "poll.h"
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


class ClientIOThread : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE{ // main body of thread execution
        // notify the thread is connected to the server
        emit connectedToServer();
        sendName();

        // First thing  CREATE THE SOCKET
        pollfd poll_set;
        bzero((char*)&poll_set, sizeof(poll_set));

        poll_set.fd = mServerSocket;
        poll_set.events = POLLIN;
        int numfds = 0;
        numfds++;
        while(mRunning)  // can be asynchronous changed
        {
            //wake on data available on socket descritpor
            poll(&poll_set,numfds,-1);
            if(poll_set.revents & POLLIN)
            {
                int bytes_to_read(0);
                // Fetch how many bytes are available
                ioctl(mServerSocket, FIONREAD, &bytes_to_read);
                if(bytes_to_read > 0) // Data available
                {
                    ssize_t bytes_read(0);
                    char* buffer (new char [bytes_to_read+10]);
                    while(bytes_read < bytes_to_read) // Read everything ioctl saw
                    {                                                                     //continue where left off
                        bytes_read = ::recv(mServerSocket,(buffer+bytes_read),bytes_to_read-bytes_read,0);
                    } // Message received
                    QByteArray message = QByteArray::fromRawData(buffer,bytes_read);
                    // Message fully received
                    serviceLobbyMessage(message); // parse it and perform function.
                    delete []buffer; // clean up buffer
                }
            }
        }
    }
public:

    ClientIOThread(QString & name, int socket_descriptor);
    ~ClientIOThread();

public slots:
    void sendName();
    void requestChallenge(QString opponent);
    void sendAcceptChallenge(QString opponent, bool accepted);
    void sendMessage(QString message);
    void sendMoveMade(bool isX, int cell);
    void disconnectFromServer();
    void sendLeavingGame();
    void sendLeavingLobby();
signals:
    void connectedToServer();
    void disconnectedFromServer();
    void clientFound(QString name);
    void clientDisconnected(QString name);
    void receivedChallenge(QString from);
    void receivedChallengeReply(QString from,bool accepted);
    void networkGameEnded();
    void moveMade(bool isX,int cell);
    void clientLeftLobby(QString name);
    void clearLobby();
    void boardUpdated(QString board, bool isTurn, bool current_piece);
    void networkGameStarted(bool playersPiece);
    void serverNameChanged( QString name);

private:
    void                                 serviceLobbyMessage( QByteArray & message);
    bool                                mRunning;
    struct sockaddr_in     mServerAddress;
    int                                    mServerSocket;
    QString                          mName;
};

#endif // CLIENTIOTHREAD_H
