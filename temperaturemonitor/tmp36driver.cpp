#include "tmp36driver.h"
#include "utility.h"
#include <QFile>

TMP36Driver::TMP36Driver()
{
    mAdcFile = new QFile(QString(ADC_FILE_PATH) + QString(ADC_INPUT_PIN));
    Q_ASSERT(mAdcFile->exists());
}

TMP36Driver::~TMP36Driver()
{
    delete mAdcFile;
}

bool TMP36Driver::getTemperature_C(float& temp)
{
    int rawAdc;
    SmartBox::readFile(mAdcFile, rawAdc);

    if (rawAdc > MAX_RAW_VALUE_OUTPUT || rawAdc < MIN_RAW_VALUE_OUTPUT) {
        return false;
    }

    temp = voltageToTemperature(rawAdc);

    return true;
}

float TMP36Driver::voltageToTemperature(int rawAdc)
{
    // input x = [0, 4096], equal to voltage(mv) [0, 1800]
    float voltage_mv = (static_cast<float>(rawAdc) /
                        static_cast<float>(MAX_RAW_VALUE_OUTPUT))
                      * static_cast<float>(MAX_OUTPUT_VOLTAGE_MV);
    return (voltage_mv - OUTPUT_VOLTAGE_OFFSET) / OUTPUT_VOLTAGE_DIVIDER;
}
