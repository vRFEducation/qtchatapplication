#ifndef CHATPROTOCOL_H
#define CHATPROTOCOL_H

#include <QByteArray>
#include <QString>



class ChatProtocol
{
public:
    enum MessageType{
        Text,
        File,
        IsTyping,
        SetName,
        SetStatus
    };

    enum Status{
        None,
        Available,
        Away,
        Busy
    };

    ChatProtocol();

    QByteArray textMessage(QString message);
    QByteArray isTypingMessage();
    QByteArray setNameMessage(QString name);
    QByteArray setStatusMessage(Status status);

    void loadData(QByteArray data);

    const QString &message() const;

    const QString &name() const;

    Status status() const;

    MessageType type() const;

private:
    QByteArray getData(MessageType type, QString data);

    MessageType _type;
    QString _message;
    QString _name;
    Status _status;

};

#endif // CHATPROTOCOL_H
