#ifndef HTTPSERVICE_H
#define HTTPSERVICE_H

#include <QObject>

class QHttpServer;
class QHttpRequest;
class QHttpResponse;
class QRegExp;

class HttpService : public QObject
{
    Q_OBJECT

public:
    HttpService();
    ~HttpService();

    void SendGetResponse(QString resp);
    void SendPostResponse(QString resp);

Q_SIGNALS:
    // Emitted when received a get request.
    void getRequest();
    // Emitted when received a post request.
    void postRequest(QString temp);
    void postRequest(QString temp, QString time);

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);

private:
    static constexpr int REG_EXP_NOT_MATCH = -1;
    constexpr static char* GET_URI = "^/get$";
    // [0-9] short for \d
    // match formats like: /set/19 or /set/19/2000
    constexpr static char* SET_URI = "^/set/([0-9]{1,2}|[0-9]{1,2}/[0-9]{4})$";

    QRegExp *mGetExp;
    QRegExp *mSetExp;
    QHttpServer *mServer;
    QHttpResponse *mResponse;
};

#endif // HTTPSERVICE_H
