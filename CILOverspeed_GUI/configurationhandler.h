#ifndef CONFIGURATIONHANDLER_H
#define CONFIGURATIONHANDLER_H

#include <QSettings>
#include <QString>

class ConfigurationHandler
{
public:
    ConfigurationHandler();
    ConfigurationHandler(QString filename);

private:
    int rated_rpm, teeth, overspeed_percent, trip_delay;

public:
    void setValues(int ratedRPM, int Teeth, int overspeedPercentage, int tripDelay);
    int getRatedRPM();
    int getTeeth();
    int getOverspeedPercent();
    int getTripDelay();
    void saveConfiguration(QString filename);
    void loadConfiguration(QString filename);
};

#endif // CONFIGURATIONHANDLER_H
