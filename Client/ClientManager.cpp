#include "ClientManager.h"

ClientManager::ClientManager(QHostAddress ip, ushort port, QObject *parent)
    : QObject{parent},
      _ip(ip),
      _port(port)
{
    setupClient();
}

void ClientManager::connectToServer()
{
    _socket->connectToHost(_ip, _port);
}

void ClientManager::sendMessage(QString message, QString receiver)
{
    _socket->write(_protocol.textMessage(message, receiver));
}

void ClientManager::sendName(QString name)
{
    _socket->write(_protocol.setNameMessage(name));
}

void ClientManager::sendStatus(ChatProtocol::Status status)
{
    _socket->write(_protocol.setStatusMessage(status));
}

void ClientManager::sendIsTyping()
{
    _socket->write(_protocol.isTypingMessage());
}

void ClientManager::sendInitSendingFile(QString fileName)
{
    _tmpFileName = fileName;
    _socket->write(_protocol.setInitSendingFileMessage(fileName));
}

void ClientManager::sendAcceptFile()
{
    _socket->write(_protocol.setAcceptFileMessage());

}

void ClientManager::sendRejectFile()
{
    _socket->write(_protocol.setRejectFileMessage());

}

void ClientManager::readyRead()
{
    auto data = _socket->readAll();
    _protocol.loadData(data);
    switch (_protocol.type()) {
    case ChatProtocol::Text:
        emit textMessageReceived(_protocol.message());
        break;
    case ChatProtocol::SetName:
        emit nameChanged(_protocol.name());
        break;
    case ChatProtocol::SetStatus:
        emit statusChanged(_protocol.status());
        break;
    case ChatProtocol::IsTyping:
        emit isTyping();
        break;
    case ChatProtocol::InitSendingFile:
        emit initReceivingFile(_protocol.name(), _protocol.fileName(), _protocol.fileSize());
        break;
    case ChatProtocol::AcceptSendingFile:
        sendFile();
        break;
    case ChatProtocol::RejectSendingFile:
        emit rejectReceivingFile();
        break;

    case ChatProtocol::ConnectionACK:
        emit connectionACK(_protocol.myName(), _protocol.clientsName());
        break;
    case ChatProtocol::NewClient:
        emit newClientConnectedToServer(_protocol.clientName());
        break;
    case ChatProtocol::ClientDisconnected:
        emit clientDisconnected(_protocol.clientName());
        break;
    case ChatProtocol::ClientName:
        emit clientNameChanged(_protocol.prevName(), _protocol.clientName());
        break;
    default:
        break;
    }
}

void ClientManager::setupClient()
{
    _socket = new QTcpSocket(this);
    connect(_socket, &QTcpSocket::connected, this, &ClientManager::connected);
    connect(_socket, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect(_socket, &QTcpSocket::readyRead, this, &ClientManager::readyRead);
}

void ClientManager::sendFile()
{
    _socket->write(_protocol.setFileMessage(_tmpFileName));
}



