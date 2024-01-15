/**
  @file
  @author Stefan Frings
*/

#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H
#include "src/Web/Controller/listcontroller.h"
#include "src/Web/Controller/playcontroller.h"
#include "src/Web/Controller/downloadcontroller.h"
#include "httprequesthandler.h"

using namespace stefanfrings;

/**
  The request mapper dispatches incoming HTTP requests to controller classes
  depending on the requested path.
*/

class RequestMapper : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(RequestMapper)
public:

    DownloadController *mDownloadController;
    ListController *mListController;
    PlayController *mPlayController;
    /**
      Constructor.
      @param parent Parent object
    */
    RequestMapper(QObject* parent=0);

    /**
      Destructor.
    */
    ~RequestMapper();

    /**
      Dispatch incoming HTTP requests to different controllers depending on the URL.
      @param request The received HTTP request
      @param response Must be used to return the response
    */
    void service(HttpRequest& request, HttpResponse& response);

};

#endif // REQUESTMAPPER_H
