#ifndef TEMPERATUREMONITOR_H
#define TEMPERATUREMONITOR_H

#include <QObject>

class QTimer;
class TemperatureSensor;
class PowerSwitchDriver;

// Run on a thread to monitor temperature
class TemperatureMonitor : public QObject
{
    Q_OBJECT
public:
    TemperatureMonitor();
    ~TemperatureMonitor();

    bool IsValidTemperature_C(float temp);

public slots:
    void getTemperature_C();
    void updateTargetTemperature(int temp);

private slots:
    void checkTemperature();

signals:
    void updateTemperature(float temp);

private:
    void startRamping();
    void stopRamping();

    // timeout 60s
    constexpr static int TEMPERATURE_UPDATE_TIMEOUT_MS = 60000;
    constexpr static int MIN_DELTA_TEMPERATURE = 2;
    int mTargetTemperature = 0;

    QTimer* mTimer;
    TemperatureSensor* mSensor;
    PowerSwitchDriver* mSwitch;
};

#endif // TEMPERATUREMONITOR_H
