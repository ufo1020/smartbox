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
    void handlePostRequest(QString temp);
    void handlePostRequest(QString temp, QString time);
    void setSchduledTemperature();

private:
    void startUpdateTemperature();
    void startScheduledTimer();

    // timeout 60s
    constexpr static int TEMPERATURE_UPDATE_TIMEOUT_MS = 60000;
    constexpr static char* INVALID_TEMPERATURE = "FF";

    // default start heating at 7:10:)
    constexpr static char* DEFAULT_TIME = "0703";
    constexpr static int MILLISECONDS_A_DAY = 86400000;

    QThread mMoniorThread; // thread running temperature monitor
    HttpService* mHttpServer = nullptr;
    TemperatureMonitor* mTempMonitor = nullptr;
    QTimer* mTimer;
    QString mSchduledTime;

    int mScheduledTemp = 20;
};

#endif // MAINCONTROLLER_H
