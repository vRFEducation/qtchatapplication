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
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    QFileInfo info(fileName);
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
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);
        QFileInfo info(fileName);
        out << SendFile << info.fileName() << info.size() << file.readAll();
        file.close();
    }
    return ba;
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
    case ClientName:
        in >> _prevName >> _clientName;
        break;
    case NewClient:
    case ClientDisconnected:
        in >> _clientName;
        break;
    case ConnectionACK:
        in >> _myName >> _clientsName;
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

const QString &ChatProtocol::myName() const
{
    return _myName;
}

const QStringList &ChatProtocol::clientsName() const
{
    return _clientsName;
}

const QString &ChatProtocol::prevName() const
{
    return _prevName;
}

const QString &ChatProtocol::clientName() const
{
    return _clientName;
}

QString ChatProtocol::receiver() const
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
