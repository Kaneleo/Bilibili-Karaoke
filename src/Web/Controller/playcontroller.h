/**
  @file
  @author Stefan Frings
*/

#ifndef PLAYCONTROLLER_H
#define PLAYCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

using namespace stefanfrings;

class PlayController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(PlayController)
public:

    /** Constructor */
    PlayController();

    /** Generates the response */
    void resumeService(HttpRequest& request, HttpResponse& response);
    void nextService(HttpRequest& request, HttpResponse& response);
    void muteService(HttpRequest& request, HttpResponse& response);

signals:
    web_resume_sig();
    web_next_sig();
    web_mute_sig();
};



#endif // PLAYCONTROLLER_H
