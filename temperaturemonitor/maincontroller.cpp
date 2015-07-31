#include "maincontroller.h"
#include "httpservice.h"
#include "temperaturemonitor.h"
#include <QDebug>
#include <QTimer>

MainController::MainController()
{
    mHttpServer = new HttpService();
    mTempMonitor = new TemperatureMonitor();
    mTimer = new QTimer();

    mTempMonitor->moveToThread(&mMoniorThread);

    // network events
    connect(mHttpServer, SIGNAL(getRequest()), this, SLOT(handleGetRequest()));
    connect(mHttpServer, SIGNAL(postRequest(int)), this, SLOT(heandlePostRequest(int)));

    // monitor thread events
    connect(this, SIGNAL(getTemperature()), mTempMonitor, SLOT(getTemperature()));
    connect(mTempMonitor, SIGNAL(getTemperatureResult(float)), this, SLOT(getTemperatureResult(float)));

    connect(this, SIGNAL(setTemperature(int)), mTempMonitor, SLOT(setTemperature(int)));

    connect(&mMoniorThread, SIGNAL(finished()), mTempMonitor, SLOT(deleteLater()));
    mMoniorThread.start();


    // checking temperature every x seconds
    connect(mTimer, SIGNAL(timeout()), mTempMonitor, SLOT(updateTemperature()));
    mTimer->start(TEMPERATURE_UPDATE_TIMEOUT_MS);

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

void MainController::heandlePostRequest(int temp)
{
    emit setTemperature(temp);
    mHttpServer->SendPostResponse(QString::number(temp));
}
