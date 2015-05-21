#include "lobbypollthread.h"
#include "tictactoeserver.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariant>
#include <QDebug>
#include <QJsonArray>
#include "unistd.h"
#include "sys/socket.h"
LobbyPollThread::LobbyPollThread()
{

}

void LobbyPollThread::incommingConnection(int socket_desc)
{
    // Add this socket to the lobby
    addClient("",socket_desc);

//    QJsonDocument lobby_list;
//    QStringList        lobby_names;
//    for(int i = 0; i < mLobby.length(); ++i)
//    {
//        lobby_names.append((*mLobby.at(i).mName));
//    }

//    QJsonObject  list;
//    QVariant variant(lobby_names);
//    list.insert("Lobby List",QJsonValue::fromVariant(variant));
//    lobby_list.setObject(list);
//    QByteArray  list_out(lobby_list.toBinaryData());
//    //qDebug() << "Sending " << list_out << list_out.length() <<  " ";
//    int list_length = list_out.length();
//    ssize_t bytes_written = send(new_client.mSocketDescriptor,
//                                                       list_out.data(),list_length ,0);
//    if(bytes_written < list_length)
//    {
//        //qDebug() << "Didnt send all the list datas!";
//        list_out.remove(0, bytes_written);
//        while(bytes_written < list_out.length())
//        {
//           int b_read = send(new_client.mSocketDescriptor,
//                                  list_out.data(), list_out.length() ,0);
//            list_out.remove(0,b_read);
//            bytes_written += b_read;
//        }
//       // qDebug() << "sent list byte count :" << bytes_written;
//    }
//    else if(bytes_written == 0)
//    {
//        emit clientDisconnected(new_client.mSocketDescriptor,new_client);
//    }
}
void LobbyPollThread::addClient(QString name,int socket_descriptor)
{
    ConnectedClient new_client;// everybody manages their own strings
    new_client.mName = QString(name.toLocal8Bit());  // deep copy
    new_client.mSocketDescriptor = socket_descriptor;
    new_client.mState = IN_LOBBY;
    mClientsInLobby.append(new_client);
    if(mField)
    {
        pollfd * temp;
        temp = new pollfd[mClientsInLobby.length()];
        for(int index(0); index < mClientsInLobby.length(); ++index)
        {
            temp[index].fd = mClientsInLobby.at(index).mSocketDescriptor;
            temp[index].events = POLLIN | POLLHUP;
        }
        delete []mField;
        mField = temp;
        mFieldSize = mClientsInLobby.length();
    }
    else
    {
        mField = new pollfd[1];
        mField[0].fd = new_client.mSocketDescriptor;
        mField[0].events = POLLIN | POLLHUP;
        mFieldSize = 1;
    }
}

void LobbyPollThread::removeClient(ConnectedClient client)
{
    int index(-1);
    if((index = mClientsInLobby.indexOf(client)) != -1)
    {
        mClientsInLobby.removeAt(index);
        mFieldSize = mClientsInLobby.length();
        if(mFieldSize > 0)
        {
            pollfd *temp = new pollfd[mFieldSize];
            for(int index(0); index < mClientsInLobby.length(); ++index)
            {
                temp[index].fd = mClientsInLobby.at(index).mSocketDescriptor;
                temp[index].events = POLLIN | POLLHUP;
            }
            delete []mField;
            mField = temp;
        }
        else
        {
            delete []mField;
            mField = nullptr;
        }
        qDebug() << client.mName << " has been removed from lobby"; // notify clients
    }
    else
    {
        mClientsInLobby.clear();
        delete []mField;
        mField = nullptr;
        mFieldSize = 0;
    }
}

void LobbyPollThread::sendLobbyMembers(ConnectedClient client)
{
    QJsonDocument doc;
    QJsonObject       obj;
    obj.insert("FuncType", LOBBY_LIST);
    QStringList   lobby;
    foreach( const ConnectedClient & currclient, mClientsInLobby)
    {
            lobby.append(currclient.mName);
    }
    QVariant variant(lobby);
    obj.insert("Lobby List",QJsonValue::fromVariant(variant));
    doc.setObject(obj);
    QByteArray   data_out(doc.toBinaryData());
    ssize_t bytes_read (send(client.mSocketDescriptor, data_out.data(),data_out.length(),0));
    if(bytes_read < data_out.length())
    {
        qDebug() << "Did not send all of lobby";
    }

}

void LobbyPollThread::notifyOthersOfJoining(ConnectedClient client)
{
    QJsonDocument doc;
    QJsonObject        obj;
    obj.insert("FuncType",CLIENT_CONNECT) ;
    obj.insert("Name",(client.mName));
    doc.setObject(obj);
    QByteArray  data_out(doc.toBinaryData());
    // has protectect to call before or after the client has been removed from lobby
    for(int index(0);  index < mClientsInLobby.length(); ++index)
    {
        ConnectedClient     current_client(mClientsInLobby.at(index));
        if(current_client == client)
        {
            // do nothing
        }
        else
        {
             int bytes_written = send(current_client.mSocketDescriptor,data_out.data(),data_out.length(),0);
             if(bytes_written < data_out.length()) // didnt write it all
             {
                   data_out.remove(0,bytes_written-1);
                   bytes_written = send(current_client.mSocketDescriptor,data_out.data(),data_out.length(),0);
             }
        }
        msleep(100);
    }
}

void LobbyPollThread::notifyOthersOfLeaving(ConnectedClient client)
{
    QJsonDocument doc;
    QJsonObject        obj;
    obj.insert("FuncType",LEFT_LOBBY) ;
    obj.insert("Opp",(client.mName));
    doc.setObject(obj);
    QByteArray  data_out(doc.toBinaryData());
    // has protectect to call before or after the client has been removed from lobby
    for(int index(0);  index < mClientsInLobby.length(); ++index)
    {
        ConnectedClient     current_client(mClientsInLobby.at(index));
        if(current_client.mName.compare(client.mName) == 0)
        {
            // do nothing
        }
        else
        {
             int bytes_written = send(current_client.mSocketDescriptor,data_out.data(),data_out.length(),0);
             if(bytes_written < data_out.length()) // didnt write it all
             {
                   data_out.remove(0,bytes_written-1);
                   bytes_written = send(current_client.mSocketDescriptor,data_out.data(),data_out.length(),0);
             }
             msleep(100);
        }
    }
}

void LobbyPollThread::serviceLobbyMessage(ConnectedClient &sender, QByteArray &message)
{
    QJsonDocument doc;
    doc = QJsonDocument::fromRawData(message.data(),message.length());
    CLIENT_FUNCTION  func;
    QJsonObject   obj(doc.object());
    func = (CLIENT_FUNCTION)obj["FuncType"].toInt();
    switch(func) // The function type notifies the server of what values to expect in obj
    {
        case CLIENT_CONNECT: // new client connected
        {
            QString name(obj["Name"].toString());
            bool found(false);
            foreach(const ConnectedClient & current_client, mClientsInLobby)
            {
                if(name.compare(current_client.mName) == 0 ) // if share the same name
                {
                    QString rand_num_str(QString::number(qrand()% 1000 + 1));
                    name.append(rand_num_str);
                    sendNameChanged(name,sender);
                    sender.mName = name;
                    found = true;
                    break;
                }
            }
            if(!found)
            {
                sender.mName.append(name.toLocal8Bit()); // make QString lengthen internal mem
            }

            sendLobbyMembers(sender);
            qDebug() <<  sender.mName << "Joined the Lobby.";
            notifyOthersOfJoining(sender);
            break;
        }
        case REQUEST_CHALLENGE:
        {
            QString player_name(obj["Opp"].toString());
            foreach(const ConnectedClient & client, mClientsInLobby)
            {
                if(client.mName.compare(player_name) == 0)
                {
                    sendChallenge(sender,client);
                }
            }
            break;
        }
        case MESSAGE: // message passing
        {
            QString message;
            message = obj["Msg"].toString();
            QByteArray data(message.toLocal8Bit());
            for(int i(0); i < mClientsInLobby.length(); ++i)
            {
                ConnectedClient client(mClientsInLobby.at(i));
                if(client == sender)
                {
                    // Do Nothing
                    continue;
                }
                else // else send to other client
                {
                    ssize_t data_sent (send(client.mSocketDescriptor,data.data() ,data.length(),0));
                    if(data_sent < data.length()) // didnt send all data?
                    {
                        qDebug() << "Did not send whole message to " << client.mName;
                    }
                }
            }
            break;
        }
        case ACCEPT_CHALLENGE:
        {
            QString player_name(obj["Opp"].toString());
            qDebug() << "Receieved challenge reply from " << sender.mName <<
                                     "For Challenge from " << player_name;
           bool accepted = obj["Acc"].toBool();
           if(accepted)
           {
                    qDebug() << "Challenge Accepted";
                    // spin up game thread
            }
            else
                qDebug() << "Challenge Denied";
            foreach(const ConnectedClient & client, mClientsInLobby)
            {

                if(client.mName.compare(player_name) == 0)
                {
                    sendChallengeAccepted(sender,client,accepted);
                    if(accepted)
                    {
                        notifyOthersOfLeaving(sender);
                        notifyOthersOfLeaving(client);
                        removeClient(sender);
                        removeClient(client);
                        emit clientsReadyForGame(client.mName, client.mSocketDescriptor, sender.mName, sender.mSocketDescriptor);
                    }
                }
            }
            break;
        }
        case LEFT_LOBBY:
        {
            int index(-1);
            if((index = mClientsInLobby.indexOf(sender)) != -1)
            {
                ConnectedClient closing_client(mClientsInLobby.takeAt(index));
                qDebug() << closing_client.mName << " Has Entered Game.";
                removeClient(closing_client);
                notifyOthersOfLeaving(closing_client);
            }
            else
            {
                qDebug() << "Did not find in lobby " << sender.mName;
            }
            break;
        }
        case CLOSING:
        {
            int index( mClientsInLobby.indexOf(sender));
            if(index != -1)
            {
                ConnectedClient closing_client(mClientsInLobby.at(index));
                qDebug() << closing_client.mName << " Has Disconnected.";
                removeClient(closing_client);
                notifyOthersOfLeaving(closing_client);
                emit clientDisconnected(closing_client.mSocketDescriptor);
            }
            else
            {
                qDebug() << "Received disconnect again from " << sender.mName;
            }
            break;
        }
    default: qDebug() << " Bad FuncType Received"; break;
    }

}

void LobbyPollThread::sendChallengeAccepted(ConnectedClient from, ConnectedClient to,  bool accepted)
{
    qDebug() <<"Sending challeng repy to " << to.mName;
    QJsonDocument doc;
    QJsonObject       obj;
    obj.insert("FuncType", ACCEPT_CHALLENGE);
    obj.insert("Opp",from.mName);
    obj.insert("Acc",accepted);
    doc.setObject(obj);
    QByteArray   data_out(doc.toBinaryData());

    ssize_t bytes_written(send(to.mSocketDescriptor, data_out.data(),data_out.length(),0));
    if(bytes_written < data_out.length())
    {
        qDebug() << "Failed to send challenge, attempting to send again";
        sendChallengeAccepted(from,to,accepted);
    }
    else
    {
        qDebug() << from.mName << " sent challenge reply to " << to.mName;
    }
}

void LobbyPollThread::sendChallenge(ConnectedClient from, ConnectedClient to)
{
    QJsonDocument doc;
    QJsonObject       obj;
    obj.insert("FuncType", REQUEST_CHALLENGE);
    obj.insert("Opp",from.mName);
    doc.setObject(obj);
    QByteArray   data_out(doc.toBinaryData());

    // just pas s the old message
    ssize_t bytes_written(send(to.mSocketDescriptor, data_out.data(),data_out.length(),0));
    if(bytes_written < data_out.length())
    {
        qDebug() << "Failed to send challenge, attempting to send again";
        sendChallenge(from,to);
    }
    else
    {
        qDebug() << from.mName << " sent challenge to " << to.mName;
    }

}

void LobbyPollThread::sendNameChanged(QString name, ConnectedClient  to)
{
    QJsonDocument doc;
    QJsonObject        obj;
    obj.insert("FuncType", NAME_CHANGE);
    obj.insert("Name",name);
    doc.setObject(obj);
    QByteArray   data_out(doc.toBinaryData());
    // just pas s the old message
    ssize_t bytes_written(send(to.mSocketDescriptor, data_out.data(),data_out.length(),0));
    if(bytes_written < data_out.length())
    {
        qDebug() << "Failed to send Change Name";
        sendNameChanged(name, to);
    }
    else
    {
        qDebug() << "Change name from " << to.mName;
    }

}

void LobbyPollThread::stopPollingLobby()
{
    while(mClientsInLobby.length() > 0)
    {
        ConnectedClient client(mClientsInLobby.takeFirst());
        qDebug() << "Closing out client " << client.mName;
        notifyOthersOfLeaving(client);
        close(client.mSocketDescriptor);
    }
    mRunning = false;
}

LobbyPollThread::~LobbyPollThread()
{
    while(mClientsInLobby.length() > 0)
    {
        ConnectedClient    client(mClientsInLobby.takeFirst());
        close(client.mSocketDescriptor);
    }
}

