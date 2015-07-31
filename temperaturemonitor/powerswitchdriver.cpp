#include "powerswitchdriver.h"
#include <QFile>
#include <QString>
#include <QByteArray>
#include <QTextStream>
#include <QDebug>

PowerSwitchDriver::PowerSwitchDriver()
{
    mGpioValueFile = new QFile(QString(GPIO_FILE_PATH) + QString(GPIO_VALUE));
    mGpioDirectionFile = new QFile(QString(GPIO_FILE_PATH) + QString(GPIO_DIRECTION));

    // /home/root/bin/init_beaglebone.sh should create those files on start up
    Q_ASSERT(mGpioValueFile->exists());
    Q_ASSERT(mGpioDirectionFile->exists());
}

PowerSwitchDriver::~PowerSwitchDriver()
{
    delete mGpioDirectionFile;
    delete mGpioValueFile;
}

void PowerSwitchDriver::switchOnPower()
{
    if (!isPowerSwitchEnabled() || isPowerOn()) {
        return;
    }
    switchPower(GPIO_Value_t::ON);
}

void PowerSwitchDriver::switchOffPower()
{
    if (!isPowerSwitchEnabled() || !isPowerOn()) {
        return;
    }
    switchPower(GPIO_Value_t::OFF);
}


bool PowerSwitchDriver::isPowerOn()
{
    return getDirection() == GPIO_Direction_t::OUT &&
            getValue() == GPIO_Value_t::ON;
}

bool PowerSwitchDriver::isPowerSwitchEnabled()
{
    // gpio should be out to be able to control on/off
    return getDirection() == GPIO_Direction_t::OUT;
}

void PowerSwitchDriver::switchPower(GPIO_Value_t value)
{
    QTextStream out(mGpioValueFile);
    out << static_cast<int>(value);
}

PowerSwitchDriver::GPIO_Direction_t PowerSwitchDriver::getDirection()
{
    Q_ASSERT(mGpioDirectionFile->open(QIODevice::ReadWrite | QIODevice::Text));
    QByteArray raw = mGpioDirectionFile->readLine();
    mGpioDirectionFile->close();

    if (raw.size() == 0) {
        return GPIO_Direction_t::UNKNOWN;
    }

    QString rawString(raw.constData());
    QString out(GPIO_DIRECTION_OUT);
    QString in(GPIO_DIRECTION_IN);

    qDebug()<<"Reading is: "<<rawString;
    if (rawString == out) {
        return GPIO_Direction_t::OUT;
    } else if (rawString == in) {
        return GPIO_Direction_t::IN;
    } else {
        return GPIO_Direction_t::UNKNOWN;
    }
}

PowerSwitchDriver::GPIO_Value_t PowerSwitchDriver::getValue()
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
