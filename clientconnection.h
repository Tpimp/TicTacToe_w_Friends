#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include "unistd.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "arpa/inet.h"
#include "sys/ioctl.h"
#include "clientiothread.h"


class ClientConnection : public QObject
{
    Q_OBJECT
public:
    ClientConnection(QObject *parent);
    ~ClientConnection();

    ClientIOThread * getConnection()
    {
        return mClientIoThread;
    }

public slots:
    void connectToServer(QString name);
    void requestChallenge(QString opponent);
    void sendMoveMade(bool isX, int cell);
    void sendAcceptChallenge(QString opponent, bool accepted);
    void sendMessage(QString message);
    void disconnectFromServer();
    void sendLeavingGame();
    void sendLeavingLobby();

signals:
    void connectedToServer();
    void disconnectedFromServer();
    void clientFound(QString name);
    void receivedChallenge(QString from);
    void receivedChallengeReply(QString from,bool accepted);
    void networkGameEnded();
    void moveMade(bool isX, int cell);
    void clientLeftLobby(QString name);
    void clearLobby();
    void boardUpdated(QString board, bool isTurn, bool current_piece);
    void networkGameStarted(bool playersPiece);
    void serverNameChanged( QString name);
private:
    ClientIOThread         * mClientIoThread;

};

#endif // CLIENTCONNECTION_H
