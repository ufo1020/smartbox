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
    if (isValidTemperature_C(mCurrentTemperature)) {
        emit getTemperatureResult(mCurrentTemperature);
    }
}

void TemperatureMonitor::setTemperature(int temp)
{
    // check new target is valid
    if (!mSensor->isValidTemperature_C(static_cast<float>(temp))) {
        return;
    }

    // check new target and current temperature are not too close
    int currentTemp = static_cast<int>(mCurrentTemperature);
    if (!isValidTemperature_C(mCurrentTemperature) || abs(currentTemp - temp) < MIN_DELTA_TEMPERATURE) {
        return;
    }

    mTargetTemperature = temp;

    if (currentTemp >= mTargetTemperature) {
        stopRamping();
    } else {
        startRamping();
    }
    emit setTemperatureResult(temp);
}

void TemperatureMonitor::updateTemperature()
{
    if (mSensor->getTemperature_C(mCurrentTemperature)) {
        if (mTargetTemperature &&
            static_cast<int>(mCurrentTemperature) - mTargetTemperature > MIN_DELTA_TEMPERATURE ) {
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

bool TemperatureMonitor::isValidTemperature_C(float temp)
{
    mSensor->isValidTemperature_C(temp);
}
