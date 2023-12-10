#include "ChatItemWidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionConnect_triggered()
{
    _client = new ClientManager();
    connect(_client , &ClientManager::connected, [this](){
        ui->centralwidget->setEnabled(true);
    });
    connect(_client, &ClientManager::disconnected, [this](){
        ui->centralwidget->setEnabled(false);
    });
    connect(_client, &ClientManager::textMessageReceived, this, &MainWindow::dataReceived);
    connect(_client, &ClientManager::isTyping, this, &MainWindow::onTyping);

    connect(ui->lnMessage, &QLineEdit::textChanged, _client, &ClientManager::sendIsTyping);
    _client->connectToServer();
}


void MainWindow::on_btnSend_clicked()
{
    auto message = ui->lnMessage->text().trimmed();
    _client->sendMessage(message);
//    ui->lstMessages->addItem(message);
    ui->lnMessage->setText("");
    ui->lnMessage->setFocus();

    auto chatWidget = new ChatItemWidget();
    chatWidget->setMessage(message, true);
    auto listWidgetItem = new QListWidgetItem();
    listWidgetItem->setSizeHint(QSize(0, 65));
    ui->lstMessages->addItem(listWidgetItem);
    ui->lstMessages->setItemWidget(listWidgetItem, chatWidget);
}

void MainWindow::dataReceived(QString message)
{
//    ui->lstMessages->addItem(data);
    auto chatWidget = new ChatItemWidget();
    chatWidget->setMessage(message);
    auto listWidgetItem = new QListWidgetItem();
    listWidgetItem->setSizeHint(QSize(0, 65));
    ui->lstMessages->addItem(listWidgetItem);
    listWidgetItem->setBackground(QColor(167, 255, 237));
    ui->lstMessages->setItemWidget(listWidgetItem, chatWidget);


}




void MainWindow::on_lnClientName_editingFinished()
{
    auto name = ui->lnClientName->text().trimmed();
    _client->sendName(name);
}


void MainWindow::on_cmbStatus_currentIndexChanged(int index)
{
    auto status = (ChatProtocol::Status)index;
    _client->sendStatus(status);
}

void MainWindow::onTyping()
{
    statusBar()->showMessage("Server is typing...", 750);
}

