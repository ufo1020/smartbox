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
    connect(mHttpServer, SIGNAL(postRequest(QString)), this, SLOT(handlePostRequest(QString)));
    connect(mHttpServer, SIGNAL(postRequest(QString, QString)), this, SLOT(handlePostRequest(QString, QString)));

    // monitor thread events
    connect(this, SIGNAL(getTemperature()), mTempMonitor, SLOT(getTemperature()));
    connect(mTempMonitor, SIGNAL(getTemperatureResult(float)), this, SLOT(getTemperatureResult(float)));

    connect(this, SIGNAL(setTemperature(int)), mTempMonitor, SLOT(setTemperature(int)));

    connect(&mMoniorThread, SIGNAL(finished()), mTempMonitor, SLOT(deleteLater()));
    mMoniorThread.start();


    // checking temperature every x seconds
    connect(mTimer, SIGNAL(timeout()), mTempMonitor, SLOT(updateTemperature()));
    mTimer->start(TEMPERATURE_UPDATE_TIMEOUT_MS);

    mSchduledTime = QString(DEFAULT_TIME);
    startScheduledTimer();
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

void MainController::handlePostRequest(QString temp)
{
    emit setTemperature(temp.toInt());
    mHttpServer->SendPostResponse(temp);
}


void MainController::handlePostRequest(QString temp, QString time)
{
    mScheduledTemp = temp.toInt();
    Q_ASSERT(time.length() == 4);
    mSchduledTime = time;
    startScheduledTimer();
    mHttpServer->SendPostResponse(temp);
}

void MainController::startScheduledTimer()
{
    // time = 2000, 20 is h, 00 is m
    QTime scheduledTime = QTime::fromString(mSchduledTime, "hhmm");
    if (!scheduledTime.isValid()) {
        qDebug()<<"input time not valid!!!";
        return;
    }

    QTime currentTime = QTime::currentTime();
    int leftTime_ms = currentTime.msecsTo(scheduledTime);
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
    // start timer again for next day
    startScheduledTimer();
    emit setTemperature(mScheduledTemp);
}
