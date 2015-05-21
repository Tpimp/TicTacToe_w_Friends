#include "tictactoegamethread.h"
#include "lobbypollthread.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariant>
#include <QDebug>
#include <QJsonArray>
TicTacToeGameThread::TicTacToeGameThread(QString playerx, int playerXSd, QString playero, int playerOSd)
    : mGame(3,3,this),mPlayerX(playerx),mPlayerO(playero),mPlayerOSd(playerOSd),mPlayerXSd(playerXSd)
{

}


void TicTacToeGameThread::serviceLobbyMessage(ConnectedClient &sender, QByteArray &message)
{
    QJsonDocument doc;
    doc = QJsonDocument::fromRawData(message.data(),message.length());
    CLIENT_FUNCTION  func;
    QJsonObject   obj(doc.object());
    func = (CLIENT_FUNCTION)obj["FuncType"].toInt();

    // get target information
    int target_socket = (sender.mSocketDescriptor == mPlayerOSd ? mPlayerXSd:mPlayerOSd);

    switch(func) // The function type notifies the server of what values to expect in obj
    {
        case MAKE_MOVE:
        {
            bool piece(obj["Piece"].toBool());
            int cell(obj["Cell"].toInt());
            bool made_move (mGame.makeMove(piece,cell));
            if(made_move)
                sendMakeMove(target_socket,piece,cell);
            break;
        }
        case LEFT_GAME:
        {
            sendLeftGame(target_socket);
            mRunning = false;

           break;
        }
        default: qDebug() << "Bad Game message received";
    }
}

void TicTacToeGameThread::sendMakeMove(int socket, bool isX, int cell)
{
    QJsonDocument doc;
    QJsonObject       obj;
    obj.insert("FuncType", UPDATE_BOARD);
    obj.insert("Board",mGame.boardToString());
    obj.insert("IsTurn", (bool)(isX ^ (socket == mPlayerOSd)));
    obj.insert("Piece", isX);
    obj.insert("Last",cell);
    doc.setObject(obj);
    QByteArray   data_out(doc.toBinaryData());
    ssize_t bytes_written(send(socket, data_out.data(),data_out.length(),0));
    if(bytes_written < data_out.length())
    {
        qDebug() << "Failed to send update board";
        sendMakeMove(socket,isX,cell);
    }
    else
    {

        qDebug() << " sent player  " << (isX ?"X ":"O ") << " to cell " << cell ;
    }
}


void TicTacToeGameThread::sendGameStarted()
{
    QJsonDocument doc;
    QJsonObject       obj;
    obj.insert("FuncType", GAME_START);
    obj.insert("Piece", true);
    doc.setObject(obj);
    QByteArray   data_out(doc.toBinaryData());
    ssize_t bytes_written(send(mPlayerXSd, data_out.data(),data_out.length(),0));
    if(bytes_written < data_out.length())
    {
        qDebug() << "Failed to send game started";
    }
    else
    {
        qDebug() << "Notified X of game started";
    }
    msleep(10);
    QJsonObject       obj2;
    QJsonDocument doc2;
    obj2.insert("FuncType", GAME_START);
    obj2.insert("Piece", false);
    doc2.setObject(obj2);

    data_out.clear();
    data_out.append(doc2.toBinaryData());
    bytes_written = send(mPlayerOSd, data_out.data(),data_out.length(),0);
    if(bytes_written < data_out.length())
    {
        qDebug() << "Failed to send game started";
        sendGameStarted();
    }
    else
    {
        qDebug() << "Notified O of game started";
    }

}


void TicTacToeGameThread::sendLeftGame(int socket)
{
    QJsonDocument doc;
    QJsonObject       obj;
    obj.insert("FuncType", LEFT_GAME);
    doc.setObject(obj);
    QByteArray   data_out(doc.toBinaryData());

    ssize_t bytes_written(send(socket, data_out.data(),data_out.length(),0));
    if(bytes_written < data_out.length())
    {
        qDebug() << "Failed to send player left game";
        sendLeftGame(socket);
    }
    else
    {
        qDebug() << " sent left game " ;
    }
}


TicTacToeGameThread::~TicTacToeGameThread()
{

}

