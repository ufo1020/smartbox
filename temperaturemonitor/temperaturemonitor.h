#ifndef TEMPERATUREMONITOR_H
#define TEMPERATUREMONITOR_H

#include <QObject>

class TemperatureSensor;
class PowerSwitchDriver;

// Run on a thread to monitor temperature
class TemperatureMonitor : public QObject
{
    Q_OBJECT
public:
    TemperatureMonitor();
    ~TemperatureMonitor();

    bool isValidTemperature_C(float temp);

public slots:
    void getTemperature();
    void setTemperature(int temp);
    void updateTemperature();

signals:
    void getTemperatureResult(float temp);
    void setTemperatureResult(int temp);

private:
    void startRamping();
    void stopRamping();


    constexpr static int MIN_DELTA_TEMPERATURE = 2;
    int mTargetTemperature = 0;
    float mCurrentTemperature = 0.0f;

    TemperatureSensor* mSensor;
    PowerSwitchDriver* mSwitch;
};

#endif // TEMPERATUREMONITOR_H
