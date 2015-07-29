#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include <QObject>

class TMP36Driver;

class TemperatureSensor
{
public:
    TemperatureSensor();
    ~TemperatureSensor();

    bool getTemperature_C(float& temp);
    bool IsSensorFaulty();
    bool isValidTemperature_C(float temp);

private:
    const static int SENSOR_FAULTY_LIMIT = 3;
    const static int MAX_TEMPERATURE = 30;
    const static int MIN_TEMPERATURE = 0;

    TMP36Driver* mTmp36;
    bool mSensorFaulty = false;
    int mInvalidCounter = 0;
    float mLastValidTemperature_C = 0.0f;
};

#endif // TEMPERATURESENSOR_H
