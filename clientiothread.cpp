#include "clientiothread.h"
#include "clientconnection.h"
#include <QDebug>
#include "sys/socket.h"
#include <netdb.h>
#include <netinet/in.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

ClientIOThread::ClientIOThread(QString & name, int socket_descriptor)
    :mServerSocket(socket_descriptor),mRunning(true),mName(name)
{

}


void  ClientIOThread::requestChallenge(QString opponent)
{
    QJsonDocument doc;
    QJsonObject  obj;
    obj.insert("FuncType",REQUEST_CHALLENGE);
    obj.insert("Opp",opponent);
    doc.setObject(obj);
    QByteArray  data_out(doc.toBinaryData());
    int bytes_written = send(mServerSocket,data_out.data(),data_out.length(),0);
    if(bytes_written < data_out.length()) // didnt write it all
    {
        data_out.remove(0,bytes_written-1);
        bytes_written = send(mServerSocket,data_out.data(),data_out.length(),0);
    }
}

void ClientIOThread::sendMessage(QString message)
{
    QJsonDocument doc;
    QJsonObject  obj;
    obj.insert("FuncType",MESSAGE);
    obj.insert("Msg",message);
    doc.setObject(obj);
    QByteArray  data_out(doc.toBinaryData());
    int bytes_written = send(mServerSocket,data_out.data(),data_out.length(),0);
    if(bytes_written < data_out.length()) // didnt write it all
    {
        data_out.remove(0,bytes_written-1);
        bytes_written = send(mServerSocket,data_out.data(),data_out.length(),0);
    }

}


void ClientIOThread::disconnectFromServer()
{
    QJsonDocument doc;
    QJsonObject  obj;
    obj.insert("FuncType",CLOSING);
    doc.setObject(obj);
    QByteArray  data_out(doc.toBinaryData());

    int bytes_written = send(mServerSocket,data_out.data(),data_out.length(),0);
    if(bytes_written < data_out.length()) // didnt write it all
    {
        data_out.remove(0,bytes_written-1);
        bytes_written = send(mServerSocket,data_out.data(),data_out.length(),0);
        qDebug() << "Sent Close";
    }
    else
    {
        qDebug() << "Sent Close";
    }
    mRunning = false;
    emit disconnectedFromServer();
}

void ClientIOThread::sendAcceptChallenge(QString opponent,bool accepted)
{
    QJsonDocument doc;
    QJsonObject  obj;
    obj.insert("FuncType",ACCEPT_CHALLENGE);
    obj.insert("Opp",opponent);
    obj.insert("Acc",accepted);
    doc.setObject(obj);
    QByteArray  data_out(doc.toBinaryData());
    int bytes_written = send(mServerSocket,data_out.data(),data_out.length(),0);
    if(bytes_written < data_out.length()) // didnt write it all
    {
        data_out.remove(0,bytes_written-1);
        bytes_written = send(mServerSocket,data_out.data(),data_out.length(),0);
    }
}


void ClientIOThread::sendLeavingGame()
{
    QJsonDocument  doc;
    QJsonObject         obj;
    obj.insert("FuncType", LEFT_GAME);
    doc.setObject(obj);
    QByteArray data_out(doc.toBinaryData());
    ssize_t data_sent (send(mServerSocket,data_out.data(),data_out.length() ,0));
    if(data_sent < data_out.length())
    {
        qDebug() << "Did send leaving game";
    }
}


void ClientIOThread::sendMoveMade(bool isX, int cell)
{
    QJsonDocument doc;
    QJsonObject  obj;
    obj.insert("FuncType",MAKE_MOVE);
    obj.insert("Piece", isX);
    obj.insert("Cell",cell);
    doc.setObject(obj);
    QByteArray  data_out(doc.toBinaryData());
    int bytes_written = send(mServerSocket,data_out.data(),data_out.length(),0);
    if(bytes_written < data_out.length()) // didnt write it all
    {
        data_out.remove(0,bytes_written-1);
        bytes_written = send(mServerSocket,data_out.data(),data_out.length(),0);
    }

}

void ClientIOThread::sendName()
{
    QJsonDocument  doc;
    QJsonObject         obj;
    obj.insert("FuncType", CLIENT_CONNECT);
    obj.insert("Name",mName);
    doc.setObject(obj);
    QByteArray data_out(doc.toBinaryData());
    ssize_t data_sent (send(mServerSocket,data_out.data(),data_out.length() ,0));
    if(data_sent < data_out.length())
    {
        qDebug() << "Did not send all data!";
    }
}


void ClientIOThread::sendLeavingLobby()
{
    QJsonDocument  doc;
    QJsonObject         obj;
    obj.insert("FuncType", LEFT_LOBBY);
    doc.setObject(obj);
    QByteArray data_out(doc.toBinaryData());
    ssize_t data_sent (send(mServerSocket,data_out.data(),data_out.length() ,0));
    if(data_sent < data_out.length())
    {
        qDebug() << "Sent left lobby";
    }
    else
    {
        sendLeavingLobby();
    }

}

void ClientIOThread::serviceLobbyMessage(QByteArray & message)
{
    QJsonDocument doc;
    doc = QJsonDocument::fromBinaryData(message);
    CLIENT_FUNCTION  func;
    QJsonObject   obj(doc.object());
    func = (CLIENT_FUNCTION)obj["FuncType"].toInt();
    switch(func)
    {
        case CLIENT_CONNECT:
        {
            if(obj.contains("Name"))
            {
                clientFound(obj["Name"].toString());
            }
            break;
        }
        case REQUEST_CHALLENGE:
        {
            if(obj.contains("Opp"))
            {
                QString from(obj["Opp"].toString());
                emit receivedChallenge(from);
            }
            else
            {
                qDebug() <<"Received badly formatted challenge";
            }
            break;
        }
        case MAKE_MOVE:
        {
            bool piece= obj["Piece"].toBool();
            int    cell = obj["Cell"].toInt();
            emit moveMade(piece, cell);
            break;
        }
        case NAME_CHANGE:
        {
            QString new_name(obj["Name"].toString());
            qDebug() << "Received name change from server to  " << new_name;
            emit serverNameChanged(new_name);
            break;
        }
        case UPDATE_BOARD:
        {
            QString board(obj["Board"].toString());
            bool     isTurn(obj["IsTurn"].toBool());
            bool     piece(obj["Piece"].toBool());
            int         lastCell(obj["Last"].toInt());
            qDebug() << "Got Move Made - " << (piece ?"X":"O")
                               <<  " @ " << lastCell;
            emit     moveMade(piece,lastCell);

            break;
        }
    case ACCEPT_CHALLENGE:
        {
            if(obj.contains("Opp"))
            {
                QString from(obj["Opp"].toString());
                bool accepted(obj["Acc"].toBool());
                emit receivedChallengeReply(from,accepted);
            }
            else
            {
                qDebug() <<"Received badly formatted challenge";
            }
            break;
        }
        case LOBBY_LIST:
        {
      //      QJsonValueRef val(obj["Lobby List"]);
   //         QJsonValue     value(val);
//            QStringList list(value.toVariant().toStringList());
            if(obj.contains("Lobby List"))
            {

                QJsonValue lobby_list_val;
                lobby_list_val = obj["Lobby List"];
                QVariant lobby_var(lobby_list_val.toVariant());
                QStringList  name_list(lobby_var.toStringList());
                emit clearLobby();
                foreach(const QString & str, name_list)
                {
                    emit clientFound(str);
                }
            }
            else
            {
                qDebug() << "Invalid Lobby List received";
            }
            break;
        }
        case GAME_START:
        {
            bool piece(obj["Piece"].toBool());
            qDebug() << "Recieved starting as player " << (piece ? "X":"O");
            emit networkGameStarted(piece);
        }
        case LEFT_LOBBY:
        {
            if(obj.contains("Opp"))
            {
                QString opponent( obj["Opp"].toString());
                clientLeftLobby(opponent);
            }
            else
            {
                qDebug() << "Bad Left Lobby received...";
            }
            break;
        }
        case LEFT_GAME:
        {
            emit networkGameEnded();
            break;
        }
        case CLIENT_DISCONNECTED:
        {
            if(obj.contains("Opp"))
            {
                QString opponent( obj["Opp"].toString());
                emit clientDisconnected(opponent);
            }
            break;
        }
        defualt: qDebug() << "Error bad function from server";break;
    }
}

ClientIOThread::~ClientIOThread()
{
    if(mRunning)
        disconnectFromServer();
    QThread::exit(0);
}

