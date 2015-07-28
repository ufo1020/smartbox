#include "maincontroller.h"
#include "httpservice.h"
#include "temperaturemonitor.h"
#include <QDebug>

MainController::MainController()
{
    mHttpServer = new HttpService();
    mTempMonitor = new TemperatureMonitor();

    mTempMonitor->moveToThread(&mMoniorThread);

    // network events
    connect(mHttpServer, SIGNAL(getRequest()), this, SLOT(handleGetRequest()));
    connect(mHttpServer, SIGNAL(postRequest(int)), this, SLOT(heandlePostRequest(int)));

    // monitor thread events
    connect(mTempMonitor, SIGNAL(updateTemperature(float)), this,
            SLOT(updateTemperature(float)));
    connect(this, &MainController::getTemperature, mTempMonitor,
            &TemperatureMonitor::getTemperature_C);

    connect(this, &MainController::updateTargetTemperature, mTempMonitor,
            &TemperatureMonitor::updateTargetTemperature);
    connect(&mMoniorThread, &QThread::finished, mTempMonitor, &QObject::deleteLater);
    mMoniorThread.start();
}

MainController::~MainController()
{
    mMoniorThread.quit();
    delete mHttpServer;
    delete mTempMonitor;
}

void MainController::handleGetRequest()
{
    emit getTemperature();
}

void MainController::updateTemperature(float temp)
{
    mHttpServer->SendGetResponse(QString::number(temp));
}

void MainController::heandlePostRequest(int temp)
{
    emit updateTargetTemperature(temp);
}
