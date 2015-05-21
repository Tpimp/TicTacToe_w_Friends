#ifndef TICTACTOESERVER_H
#define TICTACTOESERVER_H

#include <QObject>
#include "sys/socket.h"
#include "sys/types.h"
#include <QList>
#include "listeningthread.h"

class TicTacToeGameThread;

class LobbyPollThread;

enum ConnectionState{
    UNDEFINED,
    IN_LOBBY,
    WAITING_INVITE_RESPONSE,
    READY_TO_JOIN,
    IN_GAME
};




struct ConnectedClient{
    QString                    mName;
    int                              mSocketDescriptor;
    ConnectionState  mState;
    ConnectedClient()
    {
        mName = nullptr;
        mSocketDescriptor = -1;
        mState = UNDEFINED;
    }
    bool operator ==(const ConnectedClient & rhs)
    {
        // compare the string pointer and the socketdescriptor
        return (rhs.mName == mName) && (rhs.mSocketDescriptor == rhs.mSocketDescriptor);
    }
};


Q_DECLARE_METATYPE(ConnectedClient)

//qRegisterMetaType<ConnectedClient>("ConnectedClient");

class TicTacToeServer : public QObject
{
    Q_OBJECT
public:
    explicit TicTacToeServer(QObject *parent = 0);
    void   startListening();
    void   shutdownServer();
    ~TicTacToeServer();

signals:
    void  newClientJoined(QString * name, ConnectedClient client);

public slots:
    void createNewGame(QString player1, int player1sd, QString player2, int player2sd);
    void gameEnded(TicTacToeGameThread *game);
    void clientDisconnected(int client);

private:
    int                                   mServerSocket;
    QString                         mIPAddress;
    QList<int>                    mConnectedSocketsList;
    ListeningThread       *mListeningThread;
    QList<TicTacToeGameThread*> mGameLobbies;
    LobbyPollThread      * mLobbyThread;

private slots:
    void beganListening(int socket_desc);

};

#endif // TICTACTOESERVER_H
