#include "temperaturemonitor.h"
#include "temperaturesensor.h"
#include "powerswitchdriver.h"

#include <QTimer>

TemperatureMonitor::TemperatureMonitor()
{
    mTimer = new QTimer();
    mSensor = new TemperatureSensor();
    mSwitch = new PowerSwitchDriver();

    connect(mTimer, SIGNAL(timeout()), this, SLOT(checkTemperature()));
}

TemperatureMonitor::~TemperatureMonitor()
{
    delete mTimer;
    delete mSensor;
    delete mSwitch;
}

void TemperatureMonitor::getTemperature_C()
{
    float temp;

    if (mSensor->GetTemperature_C(temp)) {
        emit updateTemperature(temp);
    }
}

void TemperatureMonitor::updateTargetTemperature(int temp)
{
    // check new target is valid
    if (!mSensor->IsValidTemperature_C(static_cast<float>(temp)))
    {
        return;
    }

    // check if new target set before
    if (temp == mTargetTemperature) {
        return;
    }

    // check new target and current temperature are not too close
    float tempReading;
    bool ok = mSensor->GetTemperature_C(tempReading);
    int currentTemp = static_cast<int>(tempReading);
    if (!ok || abs(currentTemp - temp) < MIN_DELTA_TEMPERATURE) {
        return;
    }

    mTargetTemperature = temp;

    if (currentTemp >= mTargetTemperature) {
        stopRamping();
    } else {
        startRamping();
    }
}

void TemperatureMonitor::checkTemperature()
{
    float currentTemp;
    if (mSensor->GetTemperature_C(currentTemp)) {
        if (static_cast<int>(currentTemp) - mTargetTemperature
                > MIN_DELTA_TEMPERATURE ) {
            // temperature reached, stop ramping
            mSwitch->SwitchOffPower();
            return;
        }
    }

    // temperature not reached, start timer later
    mTimer->start(TEMPERATURE_UPDATE_TIMEOUT_MS);
}

void TemperatureMonitor::startRamping()
{
    mSwitch->SwitchOnPower();

    mTimer->start(TEMPERATURE_UPDATE_TIMEOUT_MS);
}

void TemperatureMonitor::stopRamping()
{
    // just turn it off, no state checking needed
    mSwitch->SwitchOffPower();
}

