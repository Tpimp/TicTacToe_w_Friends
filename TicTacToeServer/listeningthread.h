#ifndef LISTENINGTHREAD_H
#define LISTENINGTHREAD_H

#include <QThread>
#include "sys/socket.h"
#include "sys/types.h"
#include <QDebug>
#include "netdb.h"
#include "poll.h"
/***************************************************
 * This class provides an asynchronous interface to listen.
 * The thread polls and sleeps on listen.
 * When a new connection request comes in,
 * The listen thread then passes the socket desc. to
 * the main thread and begins listening again.
 * *************************************************/

#define POLL_SIZE 32

class ListeningThread : public QThread
{
    Q_OBJECT // allows signals and slots (macro)
    void run() Q_DECL_OVERRIDE{ // overload the  run method

        // First thing  CREATE THE SOCKET
        int portnumber;
        sockaddr_in   server_addr;
        pollfd poll_set;
       // create the server socket
       int ret_val( socket(AF_INET,SOCK_STREAM,0));
       if(ret_val != 1)
       {
            portnumber = 40582; // set port
            // clear serveraddr
            bzero((char*)&server_addr, sizeof(server_addr));
            mSocketDescriptor = ret_val; // store the socket descriptor

            // fill server address
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            server_addr.sin_port = htons(portnumber);

            // bind to the socket
            ret_val = bind(mSocketDescriptor,(struct sockaddr *)&server_addr,sizeof(server_addr));
            if(ret_val  != 0)
            { //errored
                qDebug() << "Failed to bind to the socket " << errno;
            }
            // Do Lookup
            //            socklen_t length;
            //            char   hostbuff[1025],servicebuff[32];
            //            length = sizeof(server_addr);
            //            if(getnameinfo((struct sockaddr*)&server_addr,length,
            //                 hostbuff,sizeof(hostbuff),servicebuff, sizeof(servicebuff),
            //                 NI_NUMERICHOST| NI_NUMERICSERV) == 0)
            //            { // succesfully fetched name info
            //                qDebug() << "Passed: /n SD: " << mSocketDescriptor
            //                                    << "/nClient IP: " << hostbuff
            //                                    << "Port Number " <<  servicebuff;
            //            }
        }
       else // errored on creating the socket
       {
           qDebug() << "Errored on creating the Server Socket " << ret_val;
       }

        int listen_val; // begin listening on the freshly bound socket
        listen_val = listen(mSocketDescriptor,5);
        if(listen_val == -1) // Failed to listen?
        {
            qDebug() << "Failed to listen";
        }
        else //else success server is up
        {
            qDebug() << "Tic Tac Toe Server has Began Listening.\n"
                                     "Waiting for clients to connect.......\n";
        }
        // built poll set list and info for Listen socket
        bzero((char*)&poll_set, sizeof(poll_set));
        poll_set.fd = mSocketDescriptor;
        poll_set.events = POLLIN;
        int numfds = 0;
        int max_fd = 0;
        numfds = 1;
        max_fd = mSocketDescriptor;
        mRunning = true;
        while(mRunning)  // can be asynchronous changed
        {
            emit startedListening(mSocketDescriptor);
            // Wait for data in
            poll(&poll_set,numfds,-1);
            if(poll_set.revents & POLLIN) // found socket request
            {
                // success accept it
                struct sockaddr  client_addr;
                socklen_t client_size(sizeof(client_addr));
                qDebug() << "Client Requesting connection.....";
                // CALL Accept to receive it
                int accept_val ( accept(mSocketDescriptor, &client_addr,&client_size));
                if(accept_val > 0) // accepted
                {
                    qDebug() << "Accepted";
                    // pass the socket descriptor to the main thread
                    emit newClientConnected(accept_val);
                }
                else
                {
                    // couldnt accept
                    qDebug() << "Server errored accept request" << accept_val << errno;
                }

            }
            else // listen errored.
            {
                qDebug() << "Server Listen request Errored"  << listen_val << " " << errno;
                msleep(200); // wait to listen again
            }

        }

    }
public:
    ListeningThread( );

    ~ListeningThread();

public slots:
    void   stopListening();

signals:
    void   newClientConnected(int socket_descriptor);
    void   startedListening(int socket_descriptor);

private:
    int     mSocketDescriptor;
    bool mRunning;
};

#endif // LISTENINGTHREAD_H
