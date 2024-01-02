/**
  @file
  @author Stefan Frings
*/

#include "formcontroller.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonObject> // { }
#include <QJsonArray> // [ ]
#include <QJsonDocument> // 解析Json
#include <QJsonValue> // int float double bool null { } [ ]
#include <QJsonParseError>
#include <QDir>
#include <QThread>
#include <QDateTime>

extern QString docrootPath;

FormController::FormController()
{
    newThread = new QThread;
    myObject = new myThread;
    myObject->setPath(QDir::currentPath()+"/Downloads");
    myObject->moveToThread(newThread);
    connect(this,SIGNAL(add_url(QString)),myObject,SLOT(addurl(QString)),Qt::DirectConnection);
    connect(myObject,SIGNAL(sig_downloadCmd_finished(QString)),this,SLOT(close_download_web_thread(QString)));
}

void FormController::service(HttpRequest& request, HttpResponse& response)
{
    QString filename="index.html";
    QByteArray array="";
    QByteArray requestUrl=request.getBody();

    if (requestUrl.size()!=0)
    {
        QJsonParseError jsonError;
        QJsonDocument doc = QJsonDocument::fromJson(requestUrl, &jsonError);
        if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
            qDebug() << "Json格式错误！" << jsonError.error;
            //return;
        }
        QJsonObject rootObj = doc.object();
        QJsonValue url = rootObj.value("inputUrl");

        qDebug()<<"url:"<<url.toString()<<endl;
        int findResult=url.toString().indexOf("https://www.bilibili.com/video/");
        //assert

        if(url.toString().indexOf("https://www.bilibili.com/video/")!=0){

            qDebug()<<"web url not suppport"<<endl;

            //addVideoFile(s);
            //mPlayer->startPlay(s.toStdString());
           // return ;
        }
        else{
            setDownloadingFlag(true);
            if(!newThread->isRunning())
                newThread->start();

            qDebug()<<"web thread Downloading"<<endl;

        emit add_url(QString(url.toString()));
        }
    }
    else
    {
        QFile file(docrootPath+filename);

        bool isok = file.open(QIODevice::ReadOnly); //只读模式打开
        if(isok == true){
         array  =  file.readAll();
         file.close();
        }
        response.setHeader("Content-Type", "text/html; charset=UTF-8");
        response.write(array,true);
    }

}


void FormController::close_download_web_thread(QString filename){
    if(newThread->isRunning())
       {
           newThread->quit();
           newThread->wait();
       }

    setDownloadingFlag(false);

      qDebug()<<QThread::currentThreadId()<<"recv work stop signal"<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}


void FormController::setDownloadingFlag(BOOL flag){
    mDownloadingFlag=flag;
}

BOOL FormController::getDownloadingFlag(){
   return mDownloadingFlag;
}
