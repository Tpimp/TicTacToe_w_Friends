#ifndef TICTACTOEGAMETHREAD_H
#define TICTACTOEGAMETHREAD_H

#include <QObject>
#include <QThread>
#include "tictactoeserver.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "sys/time.h"
#include <QDebug>
#include "netdb.h"
#include "poll.h"
#include <netinet/in.h>
#include "sys/ioctl.h"
#include "tictactoegame.h"

/***********************************************
 * TicTacToeGame thread monitors two
 * sockets in the "GameLobby". When messages
 * are received they are parsed and then passed on
 ***********************************************/

class TicTacToeGameThread : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE{

        mRunning  = true;

        char read_buffer[1200];
        QByteArray current_message;
        ConnectedClient   current_client;
        ssize_t  bytes_read(0);
        int count(-1);
        mGame.setupNewGame();
        sendGameStarted();
        while(mRunning)
        {
               //check something from X player for data
               count = -1;
               ioctl(mPlayerXSd, FIONREAD, &count);
               if(count > 0) // have data from X Player
               {
                   // setup struct to equal the current player (for checking)
                    current_client.mSocketDescriptor = mPlayerXSd;
                    current_client.mName = mPlayerX;
                    current_client.mState = IN_GAME;
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
               count = -1;
               //check something from O player for data
               ioctl(mPlayerOSd, FIONREAD, &count);
               if(count > 0) // have data from O Player
               {
                   // setup struct to equal the current player (for checking)
                    current_client.mSocketDescriptor = mPlayerOSd;
                    current_client.mName = mPlayerO;
                    current_client.mState = IN_GAME;
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
               msleep(45); // sleep for a bit, messages dont come that fast
        }
        emit gameEnded(this); // Game ended!
    }
public:
    explicit TicTacToeGameThread(QString playerx, int playerXSd, QString playero, int playerOSd);
    ~TicTacToeGameThread();
    QString   mPlayerX;
    QString   mPlayerO;
    int             mPlayerXSd;
    int             mPlayerOSd;
    bool         mRunning;

public slots:
    void      sendMakeMove(int socket, bool isX, int cell);
    void      sendLeftGame(int socket);
    void      sendGameStarted();
signals:
    void      gameEnded( TicTacToeGameThread * game);
private:
        void              serviceLobbyMessage(ConnectedClient &  sender, QByteArray & message);
        TicTacToeGame     mGame;

};

#endif // TICTACTOEGAMETHREAD_H
