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
    connect(this, SIGNAL(&MainController::getTemperature), mTempMonitor,
            SLOT(&TemperatureMonitor::getTemperature));
    connect(mTempMonitor, SIGNAL(getTemperatureResult(float)), this,
            SLOT(getTemperatureResult(float)));


    connect(this, SIGNAL(&MainController::setTemperature), mTempMonitor,
            SLOT(&TemperatureMonitor::setTemperature));

    connect(&mMoniorThread, SIGNAL(&QThread::finished), mTempMonitor, SLOT(&QObject::deleteLater));
    mMoniorThread.start();


    // checking temperature every x seconds
    connect(mTimer, SIGNAL(timeout()), this, SLOT(&TemperatureMonitor::updateTemperature()));
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
}
