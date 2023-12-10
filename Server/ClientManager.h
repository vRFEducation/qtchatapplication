#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "ChatProtocol.h"

#include <QObject>
#include <QTcpSocket>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress ip = QHostAddress::LocalHost, ushort port = 4500, QObject *parent = nullptr);
    explicit ClientManager(QTcpSocket *client, QObject *parent = nullptr);

    void connectToServer();

    void sendMessage(QString message);
    void sendName(QString name);
    void sendStatus(ChatProtocol::Status status);
    QString name() const;
public:
    void sendIsTyping();

signals:
    void connected();
    void disconnected();
//    void dataReceived(QByteArray data);
    void textMessageReceived(const QString message);
    void isTyping();
    void nameChanged(QString name);
    void statusChanged(ChatProtocol::Status status);

private slots:
    void readyRead();

private: //fields
    QTcpSocket *_socket;
    QHostAddress _ip;
    ushort _port;
    ChatProtocol _protocol;

private: //methods
     void setupClient();
};

#endif // CLIENTMANAGER_H
