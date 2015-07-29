#include "temperaturemonitor.h"
#include "temperaturesensor.h"
#include "powerswitchdriver.h"

#include <QDebug>

TemperatureMonitor::TemperatureMonitor()
{

    mSensor = new TemperatureSensor();
    mSwitch = new PowerSwitchDriver();

}

TemperatureMonitor::~TemperatureMonitor()
{
    delete mSensor;
    delete mSwitch;
}

void TemperatureMonitor::getTemperature()
{
    float temp;

    if (mSensor->getTemperature_C(temp)) {
        emit getTemperatureResult(temp);
    }
}

void TemperatureMonitor::setTemperature(int temp)
{
    // check new target is valid
    if (!mSensor->isValidTemperature_C(static_cast<float>(temp)))
    {
        return;
    }

    // check if new target set before
    if (temp == mTargetTemperature) {
        return;
    }

    // check new target and current temperature are not too close
    float tempReading;
    bool ok = mSensor->getTemperature_C(tempReading);
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

void TemperatureMonitor::updateTemperature()
{
    float currentTemp;
    if (mSensor->getTemperature_C(currentTemp)) {
        if (static_cast<int>(currentTemp) - mTargetTemperature
                > MIN_DELTA_TEMPERATURE ) {
            // temperature reached, stop ramping
            mSwitch->switchOffPower();
            return;
        }
    }
}

void TemperatureMonitor::startRamping()
{
    qDebug() << "Turn on power";
    mSwitch->switchOnPower();
}

void TemperatureMonitor::stopRamping()
{
    qDebug() << "Turn off power"; 
    // just turn it off, no state checking needed
    mSwitch->switchOffPower();
}

