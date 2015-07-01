#ifndef TMP36DRIVER_H
#define TMP36DRIVER_H

class QFile;

class TMP36Driver
{
public:
    TMP36Driver();
    ~TMP36Driver();

    bool GetTemperature_C(float& temp);

private:
    float voltageToTemperature(int voltage_mv);

    // refer to:
    // https://learn.adafruit.com/tmp36-temperature-sensor/overview
    constexpr static int MAX_OUTPUT_VOLTAGE_MV = 1800;
    constexpr static int MIN_OUTPUT_VOLTAGE_MV = 0;
    constexpr static float OUTPUT_VOLTAGE_OFFSET = 500.0f;
    constexpr static float OUTPUT_VOLTAGE_DIVIDER = 10.0f;

    constexpr static char* ADC_INPUT_PIN = "AIN1";
    constexpr static char* ADC_FILE_PATH = "/sys/devices/ocp.3/helper.15/";

    QFile* mAdcFile = nullptr;
};

#endif // TMP36DRIVER_H
