#include "ClientChatWidget.h"
#include "ui_ClientChatWidget.h"

#include <QMessageBox>
#include <QDesktopServices>

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
    connect(_client, &ClientManager::nameChanged, this, &ClientChatWidget::onClientNameChanged);
    connect(_client, &ClientManager::statusChanged, this, &ClientChatWidget::statusChanged);
    connect(_client, &ClientManager::initReceivingFile, this, &ClientChatWidget::onInitReceivingFile);
    connect(_client, &ClientManager::fileSaved, this, &ClientChatWidget::onFileSaved);
    connect(ui->lnMessage, &QLineEdit::textChanged, _client, &ClientManager::sendIsTyping);

    dir.mkdir(_client->name());
    dir.setPath("./" + _client->name());
}

void ClientChatWidget::disconnect()
{
    _client->disconnectFromHost();
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

void ClientChatWidget::textMessageReceived(QString message, QString receiver)
{
    if (receiver == "Server" || receiver == "All") {
        ui->lstMessages->addItem(message);
    }
    if(receiver != "Server"){
        emit textForOtherClients(message, receiver, _client->name());
    }
}

void ClientChatWidget::onTyping()
{
    emit isTyping(QString("%1 is typing...").arg(_client->name()));
}


void ClientChatWidget::onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize)
{
    auto message = QString("Client (%1) wants to send a file. Do you want to accept it?\nFile Name:%2\nFile Size: %3 bytes")
            .arg(clientName, fileName)
            .arg(fileSize);
    auto result = QMessageBox::question(this, "Receiving File", message);
    if (result == QMessageBox::Yes) {
        _client->sendAcceptFile();
    } else {
        _client->sendRejectFile();
    }

}

void ClientChatWidget::onFileSaved(QString path)
{
    auto message = QString("File saved here:\n%1").arg(path);
    QMessageBox::information(this, "File saved", message);
}

void ClientChatWidget::on_lblOpenFolder_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(_client->name()));
}

void ClientChatWidget::onClientNameChanged(QString prevName, QString name)
{
    QFile::rename(dir.canonicalPath(), name);
    emit clientNameChanged(prevName, name);
}

