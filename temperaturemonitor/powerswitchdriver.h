#ifndef POWERSWITCHDRIVER_H
#define POWERSWITCHDRIVER_H

class QFile;

class PowerSwitchDriver
{
public:
    PowerSwitchDriver();
    ~PowerSwitchDriver();

    void switchOnPower();
    void switchOffPower();
    bool isPowerOn();

private:
    enum class GPIO_Direction_t
    {
        UNKNOWN = 0,
        OUT,
        IN,
        COUNT
    };

    enum class GPIO_Value_t
    {
        UNKNOWN = 0,
        OFF,
        ON,
        COUNT
    };

    bool isPowerSwitchEnabled();

    void switchPower(GPIO_Value_t value);

    GPIO_Direction_t getDirection();
    GPIO_Value_t getValue();

    // use gpio pin 60
    constexpr static char* GPIO_FILE_PATH = "/sys/class/gpio/gpio60/";
    constexpr static char* GPIO_VALUE = "value";
    constexpr static char* GPIO_DIRECTION = "direction";
    constexpr static char* GPIO_DIRECTION_OUT = "out";
    constexpr static char* GPIO_DIRECTION_IN = "in";


    QFile* mGpioDirectionFile = nullptr;
    QFile* mGpioValueFile = nullptr;
};

#endif // POWERSWITCHDRIVER_H
