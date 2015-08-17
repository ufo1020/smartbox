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

    if (!isValidTemperature_C(mCurrentTemperature)) {
        return;
    }

    qDebug()<<"target temp:"<<temp;
    mTargetTemperature = static_cast<float>(temp);

//    if (currentTemp > temp || abs(currentTemp - temp) < MIN_DELTA_TEMPERATURE) {
//        stopRamping();
//        return;
//    }
//    qDebug()<<"target temp:"<<temp;
//    mTargetTemperature = static_cast<float>(temp);

//    // target larger than current, start ramping
//    startRamping();
    updateTemperature();
}

void TemperatureMonitor::updateTemperature()
{
    if (mSensor->getTemperature_C(mCurrentTemperature)) {
        qDebug()<<mCurrentTemperature;
        if (mTargetTemperature > 0.0f) {
            if ((mCurrentTemperature - mTargetTemperature) > MIN_DELTA_TEMPERATURE) {
                // temperature reached, stop ramping
                stopRamping();            
            } else if (mTargetTemperature - mCurrentTemperature > MIN_DELTA_TEMPERATURE) {
                startRamping();
            }
        }
    }
}

void TemperatureMonitor::startRamping()
{
    if (!mSwitch->isPowerOn()) {
        qDebug() << "Turn on power";
        mSwitch->switchOnPower();
    }
}

void TemperatureMonitor::stopRamping()
{
    if (mSwitch->isPowerOn()) {
        qDebug() << "Turn off power"; 
        // just turn it off, no state checking needed
        mSwitch->switchOffPower();
    }
}

bool TemperatureMonitor::isValidTemperature_C(float temp)
{
    return mSensor->isValidTemperature_C(temp);
}
