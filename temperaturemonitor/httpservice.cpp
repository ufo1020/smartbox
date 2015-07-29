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
    mPostExp = new QRegExp(QString(POST_URI));
}

HttpService::~HttpService()
{
    delete mServer;
    delete mGetExp;
    delete mPostExp;
}


void HttpService::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    mResponse = resp;
    if( mGetExp->indexIn(req->path()) != REG_EXP_NOT_MATCH )
    {
        // get request
        emit getRequest();

    } else if ( mPostExp->indexIn(req->path()) != REG_EXP_NOT_MATCH) {
        // post request
        QString temp = mPostExp->capturedTexts()[1];
        bool ok;
        int targetTemp = temp.toInt(&ok);
        if (ok) {
            emit postRequest(temp.toInt());
        } else {
            resp->writeHead(400);
            resp->end(QByteArray("Target temperature is wrong!"));
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

    QString body = tr("<html><head><title>Get Temperature</title></head><body><h1>%1!</h1></body></html>");
    qDebug()<<"get responst:"<<body;
    mResponse->end(body.arg(resp).toUtf8());
}

void HttpService::SendPostResponse(QString resp)
{
    Q_ASSERT(mResponse != nullptr);
    mResponse->setHeader("Content-Type", "text/html");
    mResponse->writeHead(200);

    QString body = tr("<html><head><title>Post Temperature</title></head><body><h1>%1!</h1></body></html>");
    mResponse->end(body.arg(resp).toUtf8());
    qDebug()<<"post responst:"<<body;
}

