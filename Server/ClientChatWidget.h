#ifndef CLIENTCHATWIDGET_H
#define CLIENTCHATWIDGET_H

#include "ClientManager.h"

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
    ~ClientChatWidget();
private slots:

    void clientDisconnected();

    void on_btnSend_clicked();

    void textMessageReceived(QString message);
    void onTyping();

signals:
    void clientNameChanged(QString name);
    void isTyping(QString message);
    void statusChanged(ChatProtocol::Status status);

private:
    Ui::ClientChatWidget *ui;
    ClientManager *_client;
};

#endif // CLIENTCHATWIDGET_H
