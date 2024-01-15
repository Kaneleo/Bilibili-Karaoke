/**
  @file
  @author Stefan Frings
*/

#ifndef DownloadController_H
#define DownloadController_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
#include <QObject>
#include "src/thread/myThread.h"

using namespace stefanfrings;
typedef int BOOL;

/**
  This controller displays a HTML form and dumps the submitted input.
*/


class DownloadController : public HttpRequestHandler{
    Q_OBJECT
    Q_DISABLE_COPY(DownloadController)
public:
    myThread *myObject;
    QThread *newThread;
    /** Constructor */
    DownloadController();

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
    BOOL getDownloadingFlag();

private:
    BOOL mDownloadingFlag;
    void setDownloadingFlag(BOOL flag);


signals:
        void add_url(QString url,int defaultP);
        void add_file(QString url);
private slots:
        void close_download_web_thread(QString filename);
};

#endif // DownloadController_H
