#include "ClientChatWidget.h"
#include "ui_ClientChatWidget.h"

ClientChatWidget::ClientChatWidget(QTcpSocket *client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientChatWidget)
{
    ui->setupUi(this);
    _client = new ClientManager(client, this);
//    connect(_client, &QTcpSocket::readyRead, this, &ClientChatWidget::dataReceived);
    connect(_client, &ClientManager::disconnected, this, &ClientChatWidget::clientDisconnected);
    connect(_client, &ClientManager::textMessageReceived, this, &ClientChatWidget::textMessageReceived);
    connect(_client, &ClientManager::isTyping, this, &ClientChatWidget::onTyping);
    connect(_client, &ClientManager::nameChanged, this, &ClientChatWidget::clientNameChanged);
    connect(_client, &ClientManager::statusChanged, this, &ClientChatWidget::statusChanged);
    connect(ui->lnMessage, &QLineEdit::textChanged, _client, &ClientManager::sendIsTyping);
}

ClientChatWidget::~ClientChatWidget()
{
    delete ui;
}

void ClientChatWidget::clientDisconnected()
{
    ui->wdgSendMessage->setEnabled(false);
}

void ClientChatWidget::on_btnSend_clicked()
{
    auto message = ui->lnMessage->text().trimmed();
    _client->sendMessage(message);
    ui->lnMessage->setText("");
    ui->lstMessages->addItem(message);
}

void ClientChatWidget::textMessageReceived(QString message)
{
    ui->lstMessages->addItem(message);
}

void ClientChatWidget::onTyping()
{
    emit isTyping(QString("%1 is typing...").arg(_client->name()));
}

