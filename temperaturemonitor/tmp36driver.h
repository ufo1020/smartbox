#ifndef TMP36DRIVER_H
#define TMP36DRIVER_H

class QFile;

class TMP36Driver
{
public:
    TMP36Driver();
    ~TMP36Driver();

    bool getTemperature_C(float& temp);

private:
    float voltageToTemperature(int rawAdc);

    // refer to:
    // https://learn.adafruit.com/tmp36-temperature-sensor/overview
    constexpr static int MAX_OUTPUT_VOLTAGE_MV = 1800;
    constexpr static int MIN_OUTPUT_VOLTAGE_MV = 0;
    constexpr static float OUTPUT_VOLTAGE_OFFSET = 500.0f;
    constexpr static float OUTPUT_VOLTAGE_DIVIDER = 10.0f;
    constexpr static int MAX_RAW_VALUE_OUTPUT = 4096;
    constexpr static int MIN_RAW_VALUE_OUTPUT = 0;

    constexpr static char* ADC_INPUT_PIN = "in_voltage1_raw";
    constexpr static char* ADC_FILE_PATH = "/sys/bus/iio/devices/iio:device0/";

    QFile* mAdcFile = nullptr;
};

#endif // TMP36DRIVER_H
