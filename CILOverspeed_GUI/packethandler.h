#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <QByteArray>
//#include <QDebug>
#include <QBuffer>
#include <QDataStream>



class PacketHandler
{

private:
        quint8 function,overspeed,trip,yy,mm,dd,hr,mn;
        quint16 speed_rated,speed_current,teeth,crc;

public:
    PacketHandler();

//#######Setter Functions#####################################
    void setFunction(int function_int);
    void setRatedSpeed(int speed_rated_int);
    void setCurrentSpeed(int speed_current_int);
    void setTeeth(int no_of_teeth_int);
    void setOverspeed(int overspeed_int);
    void setTrip(int trip_int);
    void setDate(int year_int,int month_int,int day_int);
    void setTime(int hr_int, int mn_int);
//############################################################

//#######Getter Functions#####################################
    int getFunction();
    int getRatedSpeed();
    int getCurrentSpeed();
    int getTeeth();
    int getOverspeed();
    int getTrip();
    int getYear();
    int getMonth();
    int getDay();
    int getHours();
    int getMinutes();
    int getCRC();
//############################################################

    QByteArray createPacket();
    void newEmptyPacket();
    void readPacket(QByteArray packet);
    quint16 calculateCRC();
    void generateCRC();
    bool checkCRC();
};

#endif // PACKETHANDLER_H
