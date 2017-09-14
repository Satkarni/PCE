#include <packethandler.h>

PacketHandler::PacketHandler()
{

}

void PacketHandler::setFunction(int function_int)
{
    function=quint8(function_int);
}

void PacketHandler::setRatedSpeed(int speed_rated_int)
{
    speed_rated=quint16(speed_rated_int);
}

void PacketHandler::setCurrentSpeed(int speed_current_int)
{
    speed_current=quint16(speed_current_int);
}

void PacketHandler::setTeeth(int no_of_teeth_int)
{
    teeth=quint16(no_of_teeth_int);
}

void PacketHandler::setOverspeed(int overspeed_int)
{
    overspeed=quint8(overspeed_int);
}

void PacketHandler::setTrip(int trip_int)
{
    trip=quint8(trip_int);
}
void PacketHandler::setDate(int year_int, int month_int, int day_int)
{
    yy=quint8(year_int);
    mm=quint8(month_int);
    dd=quint8(day_int);
}

void PacketHandler::setTime(int hr_int, int mn_int)
{
    hr=quint8(hr_int);
    mn=quint8(mn_int);
}

int PacketHandler::getFunction()
{
    return(int(function));
}

int PacketHandler::getRatedSpeed()
{
    return(int(speed_rated));
}

int PacketHandler::getCurrentSpeed()
{
    return(int(speed_current));
}

int PacketHandler::getTeeth()
{
    return(int(teeth));
}

int PacketHandler::getOverspeed()
{
    return(int(overspeed));
}

int PacketHandler::getTrip()
{
    return(int(trip));
}

int PacketHandler::getYear()
{
    return(int(yy));
}

int PacketHandler::getMonth()
{
    return(int(mm));
}

int PacketHandler::getDay()
{
    return(int(dd));
}

int PacketHandler::getHours()
{
    return(int(hr));
}

int PacketHandler::getMinutes()
{
    return(int(mn));
}

int PacketHandler::getCRC()
{
    return(int(crc));
}

QByteArray PacketHandler::createPacket()
{
    QByteArray packet;
    QDataStream out(&packet,QIODevice::WriteOnly);
    out << function;
    out << speed_rated;
    out << speed_current;
    out << teeth;
    out << overspeed;
    out << trip;
    out << yy;
    out << mm;
    out << dd;
    out << hr;
    out << mn;
    out << crc;
    return packet;
}

void PacketHandler::newEmptyPacket()
{
    this->setFunction(0);
    this->setCurrentSpeed(0);
    this->setRatedSpeed(0);
    this->setTeeth(0);
    this->setOverspeed(0);
    this->setTrip(0);
    this->setDate(0,0,0);
    this->setTime(0,0);
}

void PacketHandler::readPacket(QByteArray packet)
{
    QDataStream in(&packet,QIODevice::ReadOnly);
    in >> function;
    in >> speed_rated;
    in >> speed_current;
    in >> teeth;
    in >> overspeed;
    in >> trip;
    in >> yy;
    in >> mm;
    in >> dd;
    in >> hr;
    in >> mn;
    in >> crc;
}

quint16 PacketHandler::calculateCRC()
{
    QByteArray data = createPacket();
    quint16 crc_temp;
    quint8 thisbyte,shift,lastbit,l,length=14;

    crc_temp=0xffff;
    for (l=0; l<length; l++)
    {
        thisbyte = (quint8)data[l];
        crc_temp = crc_temp^thisbyte;
        for (shift=1; shift<=8; shift++)
        {
            lastbit = crc_temp & 1;
            crc_temp = (crc_temp >> 1) & 0x7fff;
            if (lastbit==1)
            {
                crc_temp = crc_temp^0xA001 ;
            }
        }
    }
    return crc_temp;
}

void PacketHandler::generateCRC()
{
    crc = calculateCRC();
}

bool PacketHandler::checkCRC()
{
    uint16_t newcrc;
    newcrc = calculateCRC();
    if(crc == newcrc)
        return true;
    else
        return false;
}

