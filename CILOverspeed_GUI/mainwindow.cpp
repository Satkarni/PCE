#include "mainwindow.h"
#include "ui_mainwindow.h"

QTimer *writeTimer, *readTimer, *refreshTimer;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    menuBar()->setNativeMenuBar(false);
    this->setFixedSize(this->size());
    serial = NULL;
    reading = 0;
    writeTimer = new QTimer(this);
    readTimer = new QTimer(this);
    refreshTimer = new QTimer(this);
    MainWindow::refreshDevice();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::checkDevice()
{
    QList<QSerialPortInfo> port = QSerialPortInfo::availablePorts();
    if(!port.isEmpty())
    {
        ui->portName->setCurrentIndex(ui->portName->findText(port[0].portName()));
        return true;
    }
    else
    {
        return false;
    }
}

bool MainWindow::checkDevice(QString portName)
{
    QSerialPortInfo port(portName);
    if(port.isNull())
        return false;
    else
        return true;
}

void MainWindow::on_connectBt_clicked()
{
    if(!checkDevice(ui->portName->currentText()))
        {
           ui->statusLabel->setText("No Device at " +ui->portName->currentText()+ ". Press F5 to Refresh (Idle)");
           return;
        }

        serial = new QSerialPort(this);

        serial->setPortName(ui->portName->currentText());

        if(serial->open(QIODevice::ReadWrite))
        {
            ui->connectBt->setEnabled(false);
            ui->disconnectBt->setEnabled(true);
            ui->statusLabel->setText("Connected to " +ui->portName->currentText()+ " (Connected)");
            ui->portName->setEnabled(false);
            connect(serial,SIGNAL(readyRead()),this,SLOT(listenToDevice()));
            connect(refreshTimer,SIGNAL(timeout()),this,SLOT(refreshDevice()));

            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setParity(QSerialPort::NoParity);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setDataBits(QSerialPort::Data8);
        }
        else if (!serial->isOpen())
        {
            ui->statusLabel->setText("Connection Error. Check device and Press F5 to refresh. (Idle)");
        }
        on_readBt_clicked();
}

void MainWindow::listenToDevice()
{
    static QByteArray data;
    PacketHandler packet;

    if (reading == 1)
    {
        data += serial->readAll();
        if(data.size() >= 16)
        {
            ui->statusLabel->setText("Read Successful (Connected)");
            refreshTimer->start(3000);
            readTimer->stop();
            packet.readPacket(data);
            processPacket(packet);
            reading = 0;
            sendACK(2);
            data.clear();
        }
    }
    else if (reading == 2)
    {
        data += serial->readAll();
        if(data.size() >= 5)
        {
            packet.readPacket(data);
            processPacket(packet);
            reading = 0;
            sendACK(5);
            data.clear();
        }
    }
    else if(reading == 3)
    {
        data += serial->readAll();
        if(data.size() >= 16)
        {
            packet.readPacket(data);
            logw->setLog(packet);
            reading = 0;
            data.clear();
        }
    }
    else
    {
        data = serial->readAll();
        switch((int)data[0])
        {
        case 2:
            ui->statusLabel->setText("Reading... (Connected)");
            reading = 1;
            break;

        case 5:
            reading = 2;
            break;

        case 4:
            reading = 3;
            break;

        case 6:
            writeTimer->stop();
            ui->statusLabel->setText("Write Successful (Connected)");
            refreshTimer->start(3000);
            data.clear();
            break;
        }
    }

}

void MainWindow::sendACK(int function)
{
    if(serial == NULL)
    {
        ui->statusLabel->setText("Error Sending ACK (Disconnected)");
        return;
    }
    else if(!serial->isOpen())
    {
        ui->statusLabel->setText("Error Sending ACK: Connection Error");
        return;
    }
    PacketHandler ACK;
    QByteArray packet;

    ACK.newEmptyPacket();
    ACK.setFunction(6);
    if (function == 2)
        ACK.setRatedSpeed(512);
    else if (function == 5)
        ACK.setRatedSpeed(1280);
    packet = ACK.createPacket();

    serial->write(packet);
}

void MainWindow::readCheck()
{
    readTimer->stop();
    if(!readSuccess)
    {
        ui->statusLabel->setText("Read Error. (Connected)");
        reading = 0;
        refreshTimer->start(3000);
    }
    else
        refreshDevice();
}

void MainWindow::processPacket(PacketHandler packet)
{
    if(packet.getFunction() == 5)
        ui->rpmLCD->display(packet.getCurrentSpeed());
    else if(packet.getFunction() == 2)
    {
        if(!packet.checkCRC())
            ui->statusLabel->setText("CRC Error (Connected)");
        else
        {
            int index;
            ui->rpmCurrent->setValue(packet.getRatedSpeed());
            ui->teethCurrent->setValue(packet.getTeeth());
            ui->ostCurrent->setValue(packet.getOverspeed());
            ui->tripCurrent->setValue(packet.getTrip());

            index = ui->rpmNew->findText(ui->rpmCurrent->text());
            if(index != -1)
                ui->rpmNew->setCurrentIndex(index);

            index = ui->teethNew->findText(ui->teethCurrent->text());
            if(index != -1)
                ui->teethNew->setCurrentIndex(index);

            index = ui->ostNew->findText(ui->ostCurrent->text());
            if(index != -1)
                ui->ostNew->setCurrentIndex(index);

            if(ui->tripCurrent->value() <= 10 && ui->tripCurrent->value() >= 0)
                ui->tripNew->setValue(ui->tripCurrent->value());
        }
    }
}

void MainWindow::refreshDevice()
{
    refreshTimer->stop();
    if(ui->portName->isEnabled())
    {
        if(!checkDevice())
        {
            ui->statusLabel->setText("Connect a Device and Press F5 to Refresh. (Idle)");
            return;
        }
    }

    if (serial!=NULL)
    {
        if(serial->isOpen())
        {
            ui->statusLabel->setText("Connected to " +ui->portName->currentText()+ " (Connected)");
        }
        else
            ui->statusLabel->setText("Ready to Connect. (Idle)");
    }
    else
        ui->statusLabel->setText("Ready to Connect. (Idle)");

}

void MainWindow::on_actionRefresh_triggered()
{
    MainWindow::refreshDevice();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_logBt_clicked()
{
  if(serial == NULL)
  {
      QMessageBox error;
      error.setText("Not Connected");
      error.setIcon(QMessageBox::Warning);
      error.exec();
      return;
  }
  if(!serial->isOpen())
  {
      QMessageBox error;
      error.setText("Not Connected");
      error.setIcon(QMessageBox::Warning);
      error.exec();
      return;
  }
  logw->refreshTable();
  logw->index = 0;
  logw->show();
  sendLogRequest();
}

void MainWindow::sendLogRequest()
{
    PacketHandler send;
    QByteArray packet;

    send.newEmptyPacket();
    send.setFunction(4);

    packet = send.createPacket();

    serial->write(packet);
}

void MainWindow::on_writeBt_clicked()
{
    if(serial == NULL)
    {
        QMessageBox error;
        error.setText("Cannot Write: Not Connected");
        error.setIcon(QMessageBox::Warning);
        error.exec();
        return;
    }
    else if (!serial->isOpen())
    {
        QMessageBox error;
        error.setText("Cannot Write: Not Connected");
        error.setIcon(QMessageBox::Warning);
        error.exec();
        return;
    }
    PacketHandler data;
    QByteArray packet;
    QMessageBox confirm;
    int confirmValue;

    data.setFunction(1);
    data.setRatedSpeed(ui->rpmNew->currentText().toInt());
    data.setCurrentSpeed(0);
    data.setTeeth(ui->teethNew->currentText().toInt());
    data.setOverspeed(ui->ostNew->currentText().toInt());
    data.setTrip(ui->tripNew->value());
    data.setDate(MainWindow::currentYear(),MainWindow::currentMonth(),MainWindow::currentDay());
    data.setTime(MainWindow::currentHours(),MainWindow::currentMinutes());
    data.generateCRC();
    packet = data.createPacket();
    if(ui->rpmLCD->value() <= 0)
    {
        confirm.setText("Please Confirm your Parameters");
        confirm.setInformativeText("Are you sure you want to write this configuration?");
        confirm.setIcon(QMessageBox::Question);
        confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        confirmValue = confirm.exec();

        switch (confirmValue)
        {
        case QMessageBox::Yes :
            writeSuccess = false;

            serial->write(packet);
            ui->statusLabel->setText("Writing to Device....(Connected)");
            refreshTimer->stop();
            connect(writeTimer,SIGNAL(timeout()),this,SLOT(writeCheck()));
            writeTimer->start(5000);
            break;

        case QMessageBox::No :
            return;
            break;

        }
    }
    else
    {
        QMessageBox rpmError;

        rpmError.setText("Engine Running");
        rpmError.setInformativeText("Please turn OFF your engine before attempting to write data");
        rpmError.setIcon(QMessageBox::Critical);
        rpmError.setStandardButtons(QMessageBox::Ok);
        rpmError.exec();
    }
}

void MainWindow::writeCheck()
{
    writeTimer->stop();
    if(!writeSuccess)
    {
        ui->statusLabel->setText("Write Error (Connected)");
        refreshTimer->start(3000);
    }
    else
        refreshDevice();
}

//#############Time Functions################

int MainWindow::currentYear()
{
    int r;
    QDateTime timeNow(QDateTime::currentDateTime());
    QString timeString = timeNow.toString("yyMMddhhmm");
    r = timeString.left(2).toInt();
    return r;
}

int MainWindow::currentMonth()
{
    int r;
    QDateTime timeNow(QDateTime::currentDateTime());
    QString timeString = timeNow.toString("yyMMddhhmm");
    r = timeString.mid(2,2).toInt();
    return r;
}

int MainWindow::currentDay()
{
    int r;
    QDateTime timeNow(QDateTime::currentDateTime());
    QString timeString = timeNow.toString("yyMMddhhmm");
    r = timeString.mid(4,2).toInt();
    return r;
}

int MainWindow::currentHours()
{
    int r;
    QDateTime timeNow(QDateTime::currentDateTime());
    QString timeString = timeNow.toString("yyMMddhhmm");
    r = timeString.mid(6,2).toInt();
    return r;
}

int MainWindow::currentMinutes()
{
    int r;
    QDateTime timeNow(QDateTime::currentDateTime());
    QString timeString = timeNow.toString("yyMMddhhmm");
    r = timeString.mid(8,2).toInt();
    return r;
}

//###########################################

void MainWindow::on_disconnectBt_clicked()
{
    if(serial->isOpen())
    {
        serial->close();
        ui->connectBt->setEnabled(true);
        ui->disconnectBt->setEnabled(false);
        ui->portName->setEnabled(true);
    }
    refreshDevice();
}

void MainWindow::on_readBt_clicked()
{
    if(serial == NULL)
    {
        QMessageBox error;
        error.setText("Cannot Read: Not Connected");
        error.setIcon(QMessageBox::Warning);
        error.exec();
        return;
    }
    else if (!serial->isOpen())
    {
        QMessageBox error;
        error.setText("Cannot Read: Not Connected");
        error.setIcon(QMessageBox::Warning);
        error.exec();
        return;
    }
    PacketHandler send;
    QByteArray packet;

    send.newEmptyPacket();
    send.setFunction(2);
    packet = send.createPacket();

    readSuccess = false;

    connect(readTimer,SIGNAL(timeout()),this,SLOT(readCheck()));
    serial->write(packet);
    readTimer->start(5000);
}

void MainWindow::on_actionSave_triggered()
{
    ConfigurationHandler config;
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Configuration File"),QDir::homePath(),tr("Configuration Files (*.ini)"));

    config.setValues(ui->rpmNew->currentText().toInt(),ui->teethNew->currentText().toInt(),ui->ostNew->currentText().toInt(),ui->tripNew->value());
    config.saveConfiguration(filename);
}

void MainWindow::on_actionLoad_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Load Configuration File"),QDir::homePath(),tr("Configuration Files (*.ini)"));
    ConfigurationHandler config(filename);
    int index, tmp;

    index = ui->rpmNew->findText(QString::number(config.getRatedRPM()));
    if(index != -1)
        ui->rpmNew->setCurrentIndex(index);

    index = ui->teethNew->findText(QString::number(config.getTeeth()));
    if(index != -1)
        ui->teethNew->setCurrentIndex(index);

    index = ui->ostNew->findText(QString::number(config.getOverspeedPercent()));
    if(index != -1)
        ui->ostNew->setCurrentIndex(index);

    tmp = config.getTripDelay();
    if(tmp <= 10 && tmp >= 0)
        ui->tripNew->setValue(tmp);
}
