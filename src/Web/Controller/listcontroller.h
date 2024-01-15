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

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
};



#endif // LISTCONTROLLER_H
