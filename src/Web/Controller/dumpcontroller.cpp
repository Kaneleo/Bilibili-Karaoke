/**
  @file
  @author Stefan Frings
*/

#include "dumpcontroller.h"
#include <QVariant>
#include <QDateTime>
#include <QThread>
#include <QFile>
#include <QFileInfo>

extern QString docrootPath;

DumpController::DumpController()
{}

void DumpController::service(HttpRequest& request, HttpResponse& response)
{
       response.setHeader("Content-Type", "text/html; charset=UTF-8");
       response.setCookie(HttpCookie("firstCookie","hello",600,QByteArray(),QByteArray(),QByteArray(),false,true));
       response.setCookie(HttpCookie("secondCookie","world",600));

       QByteArray body("<html><body>");
       body.append("<b>Request:</b>");
       body.append("<br>Method: ");
       body.append(request.getMethod());
       body.append("<br>Path: ");
       body.append(request.getPath());
       body.append("<br>Version: ");
       body.append(request.getVersion());

       body.append("<p><b>Headers:</b>");
       #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
           QMultiMapIterator<QByteArray,QByteArray> i(request.getHeaderMap());
       #else
           QMapIterator<QByteArray,QByteArray> i(request.getHeaderMap());
       #endif
       while (i.hasNext())
       {
           i.next();
           body.append("<br>");
           body.append(i.key());
           body.append("=");
           body.append(i.value());
       }

       body.append("<p><b>Parameters:</b>");

       #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
           i=QMultiMapIterator<QByteArray,QByteArray>(request.getParameterMap());
       #else
           i=QMapIterator<QByteArray,QByteArray>(request.getParameterMap());
       #endif
       while (i.hasNext())
       {
           i.next();
           body.append("<br>");
           body.append(i.key());
           body.append("=");
           body.append(i.value());
       }

       body.append("<p><b>Cookies:</b>");
       QMapIterator<QByteArray,QByteArray> i2 = QMapIterator<QByteArray,QByteArray>(request.getCookieMap());
       while (i2.hasNext())
       {
           i2.next();
           body.append("<br>");
           body.append(i2.key());
           body.append("=");
           body.append(i2.value());
       }

       body.append("<p><b>Body:</b><br>");
       body.append(request.getBody());

       body.append("</body></html>");
       response.write(body,true);

//    QString filename="index.html";
//    QByteArray array="";
//    if (request.getParameter("urlAddr").size()!=0)
//    {
//        //post
//        qDebug()<<"url:"<<request.getParameter("urlAddr")<<endl;
//    }
//    //else
//    {
//        QFile file(docrootPath+filename);

//        bool isok = file.open(QIODevice::ReadOnly); //只读模式打开
//        if(isok == true){
//          array  =  file.readAll();
//        }
//        file.close();

//        response.setHeader("Content-Type", "text/html; charset=UTF-8");
//        response.write(array,true);
//    }
}
