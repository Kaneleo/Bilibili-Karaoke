/**
  @file
  @author Stefan Frings
*/

#include <QCoreApplication>
#include "global.h"
#include "requestmapper.h"
//#include "filelogger.h"
#include "staticfilecontroller.h"
#include "src/Web/Controller/listcontroller.h"
#include "src/Web/Controller/playcontroller.h"
#include "src/Web/Controller/downloadcontroller.h"
//#include "templatecontroller.h"

//#include "controller/fileuploadcontroller.h"
//#include "controller/sessioncontroller.h"
//#include "controller/logincontroller.h"

RequestMapper::RequestMapper(QObject* parent)
    :HttpRequestHandler(parent)
{
    qDebug("RequestMapper: created");
    mDownloadController = new DownloadController();
    mListController = new ListController();
    mPlayController = new PlayController();
}


RequestMapper::~RequestMapper()
{
    qDebug("RequestMapper: deleted");
}


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());

    // For the following pathes, each request gets its own new instance of the related controller.

    if (path.startsWith("/play"))
    {        
        if (path.startsWith("/play/resume"))
            mPlayController->resumeService(request, response);
        else if (path.startsWith("/play/next"))
            mPlayController->nextService(request, response);
        else if (path.startsWith("/play/mute"))
            mPlayController->muteService(request, response);
        else staticFileController->service(request, response);
    }

    else if (path.startsWith("/list"))
    {
        if (path.startsWith("/list/moveToNext")){
            mListController->moveToNext(request, response);
        }

        else
            mListController->service(request, response);

    }

    else if (path.startsWith("/download"))
    {
        mDownloadController->service(request, response);
    }

//    else if (path.startsWith("/file"))
//    {
//        FileUploadController().service(request, response);
//    }

//    else if (path.startsWith("/session"))
//    {
//        SessionController().service(request, response);
//    }

//    else if (path.startsWith("/login"))
//    {
//        LoginController().service(request, response);
//    }

    // All other pathes are mapped to the static file controller.
    // In this case, a single instance is used for multiple requests.
    else
    {
        staticFileController->service(request, response);
    }

    qDebug("RequestMapper: finished request");

    // Clear the log buffer
//    if (logger)
//    {
//       logger->clear();
//    }
}
