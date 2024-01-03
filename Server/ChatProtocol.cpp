#include "ChatProtocol.h"

#include <QFileInfo>
#include <QIODevice>

ChatProtocol::ChatProtocol()
{

}

QByteArray ChatProtocol::textMessage(QString message, QString receiver)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << Text << receiver << message;
    return ba;
}

QByteArray ChatProtocol::isTypingMessage()
{
    return getData(IsTyping, "");
}

QByteArray ChatProtocol::setNameMessage(QString name)
{
    return getData(SetName, name);
}

QByteArray ChatProtocol::setStatusMessage(Status status)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << SetStatus << status;
    return ba;
}

QByteArray ChatProtocol::setInitSendingFileMessage(QString fileName)
{
    QByteArray ba;
    QFileInfo info(fileName);
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << InitSendingFile << info.fileName() << info.size();
    return ba;
}

QByteArray ChatProtocol::setAcceptFileMessage()
{
    return getData(AcceptSendingFile, "");

}

QByteArray ChatProtocol::setRejectFileMessage()
{
    return getData(RejectSendingFile, "");

}

QByteArray ChatProtocol::setFileMessage(QString fileName)
{
    QByteArray ba;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QFileInfo info(fileName);
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);
        out << SendFile << info.fileName() << info.size() << file.readAll() ;
        file.close();
    }
    return ba;
}

QByteArray ChatProtocol::setClientNameMessage(QString prevName, QString name)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << ClientName << prevName << name;
    return ba;
}

QByteArray ChatProtocol::setConnectionACKMessage(QString clientName, QStringList otherClients)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << ConnectionACK << clientName << otherClients;
    return ba;
}

QByteArray ChatProtocol::setNewClientMessage(QString clientName)
{
    return getData(NewClient, clientName);
}

QByteArray ChatProtocol::setClinetDisconnectedMessage(QString clientName)
{
    return getData(ClientDisconnected, clientName);
}

void ChatProtocol::loadData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_6_0);
    in >> _type;
    switch (_type) {
    case Text:
        in >> _receiver >> _message;
        break;
    case SetName:
        in >> _name;
        break;
    case SetStatus:
        in >> _status;
        break;
    case InitSendingFile:
        in >> _fileName >> _fileSize;
        break;
    case SendFile:
        in >> _fileName >> _fileSize >> _fileData;
        break;
    default:
        break;
    }
}

QByteArray ChatProtocol::getData(MessageType type, QString data)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << type << data;
    return ba;
}

const QString &ChatProtocol::receiver() const
{
    return _receiver;
}

const QByteArray &ChatProtocol::fileData() const
{
    return _fileData;
}

qint64 ChatProtocol::fileSize() const
{
    return _fileSize;
}

const QString &ChatProtocol::fileName() const
{
    return _fileName;
}

ChatProtocol::MessageType ChatProtocol::type() const
{
    return _type;
}

ChatProtocol::Status ChatProtocol::status() const
{
    return _status;
}

const QString &ChatProtocol::name() const
{
    return _name;
}

const QString &ChatProtocol::message() const
{
    return _message;
}
