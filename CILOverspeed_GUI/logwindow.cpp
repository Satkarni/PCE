#include "logwindow.h"
#include "ui_logwindow.h"

LogWindow::LogWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
}

LogWindow::~LogWindow()
{
    delete ui;
}

void LogWindow::setLog(PacketHandler logPacket)
{
    log = logPacket;
    printLog();
}
void LogWindow::refreshTable()
{
    ui->logTable->clearContents();
}

QString LogWindow::dateTimeString(int year, int month, int day, int hours, int minutes)
{
    QString dateTime = QString("20%1-%2-%3  %4:%5")
            .arg(year)
            .arg(month,2,10,QChar('0'))
            .arg(day,2,10,QChar('0'))
            .arg(hours)
            .arg(minutes,2,10,QChar('0'));
    return dateTime;
}

void LogWindow::printLog()
{
    QTableWidgetItem *dateTime = new QTableWidgetItem;
    QTableWidgetItem *ratedSpeed = new QTableWidgetItem;
    QTableWidgetItem *teeth = new QTableWidgetItem;
    QTableWidgetItem *overspeed = new QTableWidgetItem;
    QTableWidgetItem *trip = new QTableWidgetItem;

    dateTime->setText(dateTimeString(log.getYear(),log.getMonth(),log.getDay(),log.getHours(),log.getMinutes()));

    ratedSpeed->setText(QString::number(log.getRatedSpeed()));
    ratedSpeed->setTextAlignment(Qt::AlignCenter);

    teeth->setText(QString::number(log.getTeeth()));
    teeth->setTextAlignment(Qt::AlignCenter);

    overspeed->setText(QString::number(log.getOverspeed()));
    overspeed->setTextAlignment(Qt::AlignCenter);

    trip->setText(QString::number(log.getTrip()));
    trip->setTextAlignment(Qt::AlignCenter);

    if(index == 0)
    {
        dateTime->setBackground(QBrush(QColor(Qt::green)));
        ratedSpeed->setBackground(QBrush(QColor(Qt::green)));
        teeth->setBackground(QBrush(QColor(Qt::green)));
        overspeed->setBackground(QBrush(QColor(Qt::green)));
        trip->setBackground(QBrush(QColor(Qt::green)));
    }

    if(index > 4)
        ui->logTable->insertRow(index);

    ui->logTable->setItem(index,0,dateTime);
    ui->logTable->setItem(index,1,ratedSpeed);
    ui->logTable->setItem(index,2,teeth);
    ui->logTable->setItem(index,3,overspeed);
    ui->logTable->setItem(index,4,trip);

    index++;

}
