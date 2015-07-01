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
    void postRequest(int temp);

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);

private:
    static constexpr int REG_EXP_NOT_MATCH = -1;
    constexpr static char* GET_URI = "^/gettemp$";
    constexpr static char* POST_URI = "^/posttemp/([0-9]{1,2})$";
    QRegExp *mGetExp;
    QRegExp *mPostExp;
    QHttpServer *mServer;
    QHttpResponse *mResponse;
};

#endif // HTTPSERVICE_H
