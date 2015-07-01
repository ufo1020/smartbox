#include "maincontroller.h"
#include "httpservice.h"
#include "temperaturemonitor.h"
#include <QDebug>

MainController::MainController()
{
    HttpService* mHttpServer = new HttpService();
    TemperatureMonitor* mTempMonitor = new TemperatureMonitor();

    mTempMonitor->moveToThread(&mMoniorThread);

    // network events
    connect(mHttpServer, SIGNAL(getRequest()), this, SLOT(handleGetRequest()));
    connect(mHttpServer, SIGNAL(postRequest(int)), this, SLOT(heandlePostRequest(int)));

    // monitor thread events
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
    float temp;
    bool ok = mTempMonitor->GetTemperature_C(temp);
    if (ok) {
        mHttpServer->SendGetResponse(QString::number(temp));
    } else {
        mHttpServer->SendGetResponse(QString(INVALID_TEMPERATURE));
       qDebug()<<"Cannot get temp, temp reading wrong";
    }
}

void MainController::heandlePostRequest(int temp)
{
    emit updateTargetTemperature(temp);
}
