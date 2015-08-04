#include "maincontroller.h"
#include "httpservice.h"
#include "temperaturemonitor.h"
#include <QDebug>
#include <QTimer>
#include <QTime>

MainController::MainController()
{
    mHttpServer = new HttpService();
    mTempMonitor = new TemperatureMonitor();
    mTimer = new QTimer();


    mTempMonitor->moveToThread(&mMoniorThread);

    // network events
    connect(mHttpServer, SIGNAL(getRequest()), this, SLOT(handleGetRequest()));
    connect(mHttpServer, SIGNAL(postRequest(QString)), this, SLOT(heandlePostRequest(QString)));
    connect(mHttpServer, SIGNAL(postRequest(QString, QString)), this, SLOT(heandlePostRequest(QString, QString)));

    // monitor thread events
    connect(this, SIGNAL(getTemperature()), mTempMonitor, SLOT(getTemperature()));
    connect(mTempMonitor, SIGNAL(getTemperatureResult(float)), this, SLOT(getTemperatureResult(float)));

    connect(this, SIGNAL(setTemperature(int)), mTempMonitor, SLOT(setTemperature(int)));

    connect(&mMoniorThread, SIGNAL(finished()), mTempMonitor, SLOT(deleteLater()));
    mMoniorThread.start();


    // checking temperature every x seconds
    connect(mTimer, SIGNAL(timeout()), mTempMonitor, SLOT(updateTemperature()));
    mTimer->start(TEMPERATURE_UPDATE_TIMEOUT_MS);

    QTime time = QTime::fromString(QString(DEFAULT_TIME), "hhmm");
    startScheduledTimer(time);
}

MainController::~MainController()
{
    mMoniorThread.quit();
    delete mHttpServer;
    if (mTimer->isActive()) {
        mTimer->stop();
    }
    delete mTimer;
    delete mTempMonitor;
}

void MainController::handleGetRequest()
{
    emit getTemperature();
}

void MainController::getTemperatureResult(float temp)
{
    char format = 'f'; //format as 9.9
    int precision = 1;
    mHttpServer->SendGetResponse(QString::number(temp, format, precision));
}

void MainController::heandlePostRequest(QString temp)
{
    emit setTemperature(temp.toInt());
    mHttpServer->SendPostResponse(temp);
}


void MainController::heandlePostRequest(QString temp, QString time)
{
    mScheduledTemp = temp.toInt();
    Q_ASSERT(time.length() == 4);
    // time = 2000, 20 is h, 00 is m
    QTime scheduledTime = QTime::fromString(time, "hhmm");
    if (!scheduledTime.isValid()) {
        qDebug()<<"input time not valid!!!";
        return;
    }
    startScheduledTimer(scheduledTime);
    mHttpServer->SendPostResponse(temp);
}

void MainController::startScheduledTimer(QTime& time)
{
    QTime currentTime = QTime::currentTime();
    int leftTime_ms = currentTime.msecsTo(time);
    // always use postive numbers, means looking for futurn time
    // for example current 20:00, scheduled 8:00, result -12
    if (leftTime_ms < 0) {
        // need to always make time to future
        leftTime_ms = MILLISECONDS_A_DAY + leftTime_ms;
    }
    qDebug()<<"left "<<leftTime_ms/1000<<"s";
    QTimer::singleShot((leftTime_ms), this,SLOT(setSchduledTemperature()));
}

void MainController::setSchduledTemperature()
{
    emit setTemperature(mScheduledTemp);
}
