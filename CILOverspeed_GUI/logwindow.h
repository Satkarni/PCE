#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QDialog>
#include <QSerialPort>
//#include <QDebug>
#include <QTableWidgetItem>
#include "packethandler.h"

/* This comment is used to test Git */

namespace Ui {
class LogWindow;
}

class LogWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LogWindow(QWidget *parent = 0);

    int index;

    void setLog(PacketHandler logPacket);

    void refreshTable();

    ~LogWindow();

private:
    Ui::LogWindow *ui;

    PacketHandler log;

    QString dateTimeString(int year, int month, int day, int hours, int minutes);

    void printLog();

private slots:

};

#endif // LOGWINDOW_H
