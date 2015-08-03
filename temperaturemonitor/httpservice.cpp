#include "httpservice.h"
#include <QRegExp>

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>

HttpService::HttpService() : mResponse(nullptr)
{
    mServer = new QHttpServer(this);
    connect(mServer, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));

    mServer->listen(QHostAddress::Any, 8080);

    mGetExp = new QRegExp(QString(GET_URI));
    mSetExp = new QRegExp(QString(SET_URI));
}

HttpService::~HttpService()
{
    delete mServer;
    delete mGetExp;
    delete mSetExp;
}


void HttpService::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    mResponse = resp;
    if( mGetExp->indexIn(req->path()) != REG_EXP_NOT_MATCH )
    {
        // get request
        emit getRequest();

    } else if ( mSetExp->indexIn(req->path()) != REG_EXP_NOT_MATCH) {
        // post request has 2 types: xx(temp) or xx/xxxx(temp and time)
        // mSetExp->capturedTexts() returns a list like:
        // ("/set/22/2000", "22/2000")
        QString input = mSetExp->capturedTexts()[1];
        QStringList list = input.split("/");

        QString temp = list[0];
        if (list.length() > 1) {
            // post request contains temp and time
            QString time = list[1];
            emit postRequest(temp, time);
        } else {
            emit postRequest(temp);
        }

    } else {
        resp->writeHead(403);
        resp->end(QByteArray("You aren't allowed here!"));
    }
}

void HttpService::SendGetResponse(QString resp)
{
    Q_ASSERT(mResponse != nullptr);
    mResponse->setHeader("Content-Type", "text/html");
    mResponse->writeHead(200);

    QString body = tr("<html><head><title>Get Temperature</title></head><body><h1>%1</h1></body></html>");
//    qDebug()<<"get responst:"<<body;
    mResponse->end(body.arg(resp).toUtf8());
}

void HttpService::SendPostResponse(QString resp)
{
    Q_ASSERT(mResponse != nullptr);
    mResponse->setHeader("Content-Type", "text/html");
    mResponse->writeHead(200);

    QString body = tr("<html><head><title>Post Temperature</title></head><body><h1>%1</h1></body></html>");
    mResponse->end(body.arg(resp).toUtf8());
//    qDebug()<<"post responst:"<<body;
}

