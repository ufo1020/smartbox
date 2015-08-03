#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QThread>

class QTimer;
class HttpService;
class TemperatureMonitor;

class MainController : public QObject
{
    Q_OBJECT

public:
    MainController();
    ~MainController();

signals:
    void getTemperature();
    void setTemperature(int temp);

private slots:
    void getTemperatureResult(float temp);
    void handleGetRequest();
    void heandlePostRequest(QString temp);
    void heandlePostRequest(QString temp, QString time);
    void setSchduledTemperature();

private:
    void startUpdateTemperature();

    // timeout 60s
    constexpr static int TEMPERATURE_UPDATE_TIMEOUT_MS = 60000;
    constexpr static char* INVALID_TEMPERATURE = "FF";

    QThread mMoniorThread; // thread running temperature monitor
    HttpService* mHttpServer = nullptr;
    TemperatureMonitor* mTempMonitor = nullptr;
    QTimer* mTimer;

    int mScheduledTemp = 0;
};

#endif // MAINCONTROLLER_H
