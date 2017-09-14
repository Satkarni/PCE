#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <windows.h>
#include <QByteArray>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>

#include "logwindow.h"
#include "packethandler.h"
#include "configurationhandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    LogWindow *logw = new LogWindow(this);

    ~MainWindow();

signals:
    void receivedLog(PacketHandler packet);

private slots:
    void on_connectBt_clicked();

    void on_actionRefresh_triggered();

    void on_actionExit_triggered();

    void on_logBt_clicked();

    void on_writeBt_clicked();

    void on_disconnectBt_clicked();

    void listenToDevice();

    void on_readBt_clicked();

    void writeCheck(); //Checks if a write is successful

    void readCheck(); //Checks if a read is successful

    void refreshDevice();

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

private:
    Ui::MainWindow *ui;

    bool writeSuccess, readSuccess;
    int reading;
    QSerialPort *serial;

    bool checkDevice();
    bool checkDevice(QString portName);
    void sendACK(int function);

    void sendLogRequest();

    int currentYear();
    int currentMonth();
    int currentDay();
    int currentHours();
    int currentMinutes();

    void processPacket(PacketHandler packet);

};

#endif // MAINWINDOW_H
