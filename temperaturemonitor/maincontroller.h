#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QThread>

class HttpService;
//class TemperatureSensor;
class TemperatureMonitor;

class MainController : public QObject
{
    Q_OBJECT

public:
    MainController();
    ~MainController();

signals:
    void updateTargetTemperature(int temp);

private slots:
    void handleGetRequest();
    void heandlePostRequest(int temp);

private:
    constexpr static char* INVALID_TEMPERATURE = "FF";

    QThread mMoniorThread; // thread running temperature monitor
    HttpService* mHttpServer = nullptr;
    TemperatureMonitor* mTempMonitor = nullptr;
};

#endif // MAINCONTROLLER_H
