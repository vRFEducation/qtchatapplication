#ifndef CHATPROTOCOL_H
#define CHATPROTOCOL_H

#include <QByteArray>
#include <QString>
#include <QStringList>


class ChatProtocol
{
public:
    enum MessageType{
        Text,
        IsTyping,
        SetName,
        SetStatus,
        InitSendingFile,
        AcceptSendingFile,
        RejectSendingFile,
        SendFile,
        ClientName,
        ConnectionACK,
        NewClient,
        ClientDisconnected

    };

    enum Status{
        None,
        Available,
        Away,
        Busy
    };

    ChatProtocol();

    QByteArray textMessage(QString message, QString receiver);
    QByteArray isTypingMessage();
    QByteArray setNameMessage(QString name);
    QByteArray setStatusMessage(Status status);
    QByteArray setInitSendingFileMessage(QString fileName);
    QByteArray setAcceptFileMessage();
    QByteArray setRejectFileMessage();
    QByteArray setFileMessage(QString fileName);

    void loadData(QByteArray data);

    const QString &message() const;

    const QString &name() const;

    Status status() const;

    MessageType type() const;

    const QString &fileName() const;

    qint64 fileSize() const;

    const QByteArray &fileData() const;

    QString receiver() const;

    const QString &clientName() const;

    const QString &prevName() const;

    const QStringList &clientsName() const;

    const QString &myName() const;

private:
    QByteArray getData(MessageType type, QString data);

    MessageType _type;
    QString _message;
    QString _name;
    Status _status;
    QString _fileName;
    qint64 _fileSize;
    QByteArray _fileData;
    QString _receiver;
    QString _clientName;
    QString _prevName;
    QStringList _clientsName;
    QString _myName;

};

#endif // CHATPROTOCOL_H
