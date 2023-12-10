#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ClientManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionConnect_triggered();

    void on_btnSend_clicked();

    void dataReceived(QString message);

    void on_lnClientName_editingFinished();

    void on_cmbStatus_currentIndexChanged(int index);

    void onTyping();

private:
    Ui::MainWindow *ui;
    ClientManager *_client;
};
#endif // MAINWINDOW_H
