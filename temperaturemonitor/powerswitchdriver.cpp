#include "powerswitchdriver.h"
#include <QFile>
#include <QString>
#include <QByteArray>
#include <QTextStream>

PowerSwitchDriver::PowerSwitchDriver()
{
    mGpioValueFile = new QFile(QString(GPIO_FILE_PATH) + QString(GPIO_VALUE));
    mGpioDirectionFile = new QFile(QString(GPIO_FILE_PATH) + QString(GPIO_DIRECTION));

    // /home/root/bin/init_gpio.sh should create those files on start up
    Q_ASSERT(mGpioValueFile->exists());
    Q_ASSERT(mGpioDirectionFile->exists());

    Q_ASSERT(mGpioValueFile->open(QIODevice::ReadWrite | QIODevice::Text));
    Q_ASSERT(mGpioDirectionFile->open(QIODevice::ReadWrite | QIODevice::Text));
}

PowerSwitchDriver::~PowerSwitchDriver()
{
    mGpioDirectionFile->close();
    mGpioValueFile->close();

    delete mGpioDirectionFile;
    delete mGpioValueFile;
}

void PowerSwitchDriver::SwitchOnPower()
{
    if (!IsPowerSwitchEnabled() || IsPowerOn()) {
        return;
    }
    SwitchPower(GPIO_Value_t::ON);
}

void PowerSwitchDriver::SwitchOffPower()
{
    if (!IsPowerSwitchEnabled() || !IsPowerOn()) {
        return;
    }
    SwitchPower(GPIO_Value_t::OFF);
}


bool PowerSwitchDriver::IsPowerOn()
{
    return GetDirection() == GPIO_Direction_t::OUT &&
            GetValue() == GPIO_Value_t::ON;
}

bool PowerSwitchDriver::IsPowerSwitchEnabled()
{
    // gpio should be out to be able to control on/off
    return GetDirection() == GPIO_Direction_t::OUT;
}

void PowerSwitchDriver::SwitchPower(GPIO_Value_t value)
{
    QTextStream out(mGpioValueFile);
    out << static_cast<int>(value);
}

PowerSwitchDriver::GPIO_Direction_t PowerSwitchDriver::GetDirection()
{
    QByteArray raw = mGpioDirectionFile->readLine();

    if (raw.size() == 0) {
        return GPIO_Direction_t::UNKNOWN;
    }

    QString rawString(raw.constData());
    QString out(GPIO_DIRECTION_OUT);
    QString in(GPIO_DIRECTION_IN);

    if (rawString == out) {
        return GPIO_Direction_t::OUT;
    } else if (rawString == in) {
        return GPIO_Direction_t::IN;
    } else {
        return GPIO_Direction_t::UNKNOWN;
    }
}

PowerSwitchDriver::GPIO_Value_t PowerSwitchDriver::GetValue()
{
    QByteArray raw = mGpioValueFile->readLine();

    if (raw.size() == 0) {
        return GPIO_Value_t::UNKNOWN;
    }

    int rawValue = raw.toInt();

    if (rawValue > static_cast<int>(GPIO_Value_t::UNKNOWN) &&
            rawValue < static_cast<int>(GPIO_Value_t::COUNT)) {
        return static_cast<GPIO_Value_t>(rawValue);
    } else {
        return GPIO_Value_t::UNKNOWN;
    }
}
