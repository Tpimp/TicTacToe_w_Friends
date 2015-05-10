#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    ClientConnection(QObject *parent);
    ~ClientConnection();

private:

};

#endif // CLIENTCONNECTION_H
