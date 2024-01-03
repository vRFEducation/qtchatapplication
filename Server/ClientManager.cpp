#include "ClientManager.h"

#include <QDir>

ClientManager::ClientManager(QHostAddress ip, ushort port, QObject *parent)
    : QObject{parent},
      _ip(ip),
      _port(port)
{
    _socket = new QTcpSocket(this);
    setupClient();
}

ClientManager::ClientManager(QTcpSocket *client, QObject *parent)
    : QObject{parent},
    _socket(client)
{
    setupClient();
}

void ClientManager::connectToServer()
{
    _socket->connectToHost(_ip, _port);
}

void ClientManager::disconnectFromHost()
{
    _socket->disconnectFromHost();
}

void ClientManager::sendMessage(QString message)
{
    _socket->write(_protocol.textMessage(message, name()));
}

void ClientManager::sendName(QString name)
{
    _socket->write(_protocol.setNameMessage(name));
}

void ClientManager::sendStatus(ChatProtocol::Status status)
{
    _socket->write(_protocol.setStatusMessage(status));
}

QString ClientManager::name() const
{
    auto id = _socket->property("id").toInt();
    auto name = _protocol.name().length() > 0 ? _protocol.name() : QString("Client (%1)").arg(id);

    return name;
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
        emit textMessageReceived(_protocol.message(), _protocol.receiver());
        break;
    case ChatProtocol::SetName:{
        auto prevName = _socket->property("clientName").toString();
        _socket->setProperty("clientName", name());
        emit nameChanged(prevName, name());
        break;
    }
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
    case ChatProtocol::SendFile:
        saveFile();
        break;
    default:
        break;
    }
}

void ClientManager::setupClient()
{
    connect(_socket, &QTcpSocket::connected, this, &ClientManager::connected);
    connect(_socket, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect(_socket, &QTcpSocket::readyRead, this, &ClientManager::readyRead);
}

void ClientManager::sendFile()
{
    _socket->write(_protocol.setFileMessage(_tmpFileName));
}

void ClientManager::saveFile()
{
    QDir dir;
    dir.mkdir(name());
    auto path = QString("%1/%2/%3_%4")
            .arg(dir.canonicalPath(), name(), QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"), _protocol.fileName());
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(_protocol.fileData());
        file.close();
        emit fileSaved(path);
    }
}

