/**
  @file
  @author Stefan Frings
*/

#ifndef LISTCONTROLLER_H
#define LISTCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

using namespace stefanfrings;

/**
  This controller lists the received HTTP request in the response.
*/

class ListController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(ListController)
public:

    /** Constructor */
    ListController();

    int requestFlag=0; //0 wait, 1 requesting, 2 success ,3 failed
    int* mCurrentIndexPtr=nullptr;
    QStringList* mVideoList;

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

signals:
    void sig_moveToNext(int item);

private slots:
    void slot_send_list(QStringList* mList, int* mIndex);
};



#endif // LISTCONTROLLER_H
