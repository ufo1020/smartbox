#include "temperaturesensor.h"
#include "tmp36driver.h"

TemperatureSensor::TemperatureSensor()
{
    mTmp36 = new TMP36Driver();
}

TemperatureSensor::~TemperatureSensor()
{
    delete mTmp36;
}

bool TemperatureSensor::GetTemperature_C(float& temp)
{

    if (!mTmp36->GetTemperature_C(temp)) {
        mInvalidCounter++;
        temp = mLastValidTemperature_C;
        return false;
    }

    mLastValidTemperature_C = temp;
    mInvalidCounter = 0;
    return true;
}

bool TemperatureSensor::IsSensorFaulty()
{
    return mInvalidCounter > SENSOR_FAULTY_LIMIT;
}

bool TemperatureSensor::IsValidTemperature_C(float temp)
{
    return temp <= MAX_TEMPERATURE && temp >= MIN_TEMPERATURE;
}

