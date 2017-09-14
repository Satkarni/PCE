#include "configurationhandler.h"

ConfigurationHandler::ConfigurationHandler()
{
    rated_rpm = -1;
    teeth = -1;
    overspeed_percent = -1;
    trip_delay = -1;
}

ConfigurationHandler::ConfigurationHandler(QString filename)
{
    QSettings configFile(filename,QSettings::IniFormat);
    configFile.sync();

    rated_rpm = configFile.value("ratedRPM",1500).toInt();
    teeth = configFile.value("Teeth",102).toInt();
    overspeed_percent = configFile.value("overspeedPercentage",10).toInt();
    trip_delay = configFile.value("tripDelay",0).toInt();
}

void ConfigurationHandler::setValues(int ratedRPM, int Teeth, int overspeedPercentage, int tripDelay)
{
   rated_rpm = ratedRPM;
   teeth = Teeth;
   overspeed_percent = overspeedPercentage;
   trip_delay = tripDelay;
}

int ConfigurationHandler::getRatedRPM()
{
    return rated_rpm;
}

int ConfigurationHandler::getTeeth()
{
    return teeth;
}

int ConfigurationHandler::getOverspeedPercent()
{
    return overspeed_percent;
}

int ConfigurationHandler::getTripDelay()
{
    return trip_delay;
}

void ConfigurationHandler::saveConfiguration(QString filename)
{
    QSettings configFile(filename,QSettings::IniFormat);

    if(rated_rpm != -1)
        configFile.setValue("ratedRPM",rated_rpm);

    if(teeth != -1)
        configFile.setValue("Teeth",teeth);

    if(overspeed_percent != -1)
        configFile.setValue("overspeedPercentage",overspeed_percent);

    if(trip_delay != -1)
        configFile.setValue("tripDelay",trip_delay);

    configFile.sync();
}

void ConfigurationHandler::loadConfiguration(QString filename)
{
    QSettings configFile(filename,QSettings::IniFormat);
    configFile.sync();

    rated_rpm = configFile.value("ratedRPM",1500).toInt();
    teeth = configFile.value("Teeth",102).toInt();
    overspeed_percent = configFile.value("overspeedPercentage",10).toInt();
    trip_delay = configFile.value("tripDelay",0).toInt();
}


