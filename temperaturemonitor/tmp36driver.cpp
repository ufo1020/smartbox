#include "tmp36driver.h"
#include <QFile>

TMP36Driver::TMP36Driver()
{
    mAdcFile = new QFile(QString(ADC_FILE_PATH) + QString(ADC_INPUT_PIN));
    Q_ASSERT(mAdcFile->exists());
    Q_ASSERT(mAdcFile->open(QIODevice::ReadOnly | QIODevice::Text));
}

TMP36Driver::~TMP36Driver()
{
    mAdcFile->close();
    delete mAdcFile;
}

bool TMP36Driver::GetTemperature_C(float& temp)
{
    QByteArray raw = mAdcFile->readLine();

    if (raw.size() == 0) {
        return false;
    }

    bool ok;

    int voltage = raw.toInt(&ok);

    if (!ok) {
        return false;
    }

    if (voltage > MAX_OUTPUT_VOLTAGE_MV || voltage < MIN_OUTPUT_VOLTAGE_MV) {
        return false;
    }

    temp = voltageToTemperature(voltage);

    return true;
}

float TMP36Driver::voltageToTemperature(int voltage_mv)
{
    return (voltage_mv - OUTPUT_VOLTAGE_OFFSET) / OUTPUT_VOLTAGE_DIVIDER;
}
