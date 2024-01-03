#ifndef CLIENTCHATWIDGET_H
#define CLIENTCHATWIDGET_H

#include "ClientManager.h"

#include <QDir>
#include <QTcpSocket>
#include <QWidget>

namespace Ui {
class ClientChatWidget;
}

class ClientChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientChatWidget(QTcpSocket *cleint, QWidget *parent = nullptr);
    void disconnect();
    ~ClientChatWidget();
private slots:

    void clientDisconnected();

    void on_btnSend_clicked();

    void textMessageReceived(QString message, QString receiver);
    void onTyping();



    void onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize);
    void onFileSaved(QString path);

    void on_lblOpenFolder_linkActivated(const QString &link);

    void onClientNameChanged(QString prevName, QString name);
signals:
    void clientNameChanged(QString prevName, QString name);
    void isTyping(QString message);
    void statusChanged(ChatProtocol::Status status);
    void textForOtherClients(QString message, QString receiver, QString sender);
private:
    Ui::ClientChatWidget *ui;
    ClientManager *_client;
    QDir dir;
};

#endif // CLIENTCHATWIDGET_H
