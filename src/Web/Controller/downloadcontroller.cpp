/**
  @file
  @author Stefan Frings
*/

#include "downloadcontroller.h"

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
#include "MainWindow.h"

extern QString docrootPath;
extern MainWindow w;


DownloadController::DownloadController()
{
    newThread = new QThread;
    myObject = new myThread;
    myObject->setPath(QDir::currentPath()+"/Downloads");
    myObject->moveToThread(newThread);
    connect(this,SIGNAL(add_url(QString,int)),myObject,SLOT(addurl(QString,int)),Qt::DirectConnection);
    connect(myObject,SIGNAL(sig_downloadCmd_finished()),this,SLOT(close_download_web_thread()));
    connect(myObject,SIGNAL(sig_download_dequeue(DownloadItem)),this,SLOT(slot_web_download_dequeue(DownloadItem)));
}

void DownloadController::service(HttpRequest& request, HttpResponse& response)
{
    QString filename="index.html";
    QByteArray array="";
    QByteArray htmlBody=request.getBody();

    if (htmlBody.size()!=0)
    {
        QJsonParseError jsonError;
        QJsonDocument doc = QJsonDocument::fromJson(htmlBody, &jsonError);
        if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
            qDebug() << "Json格式错误！" << jsonError.error;
            //return;
        }
        QJsonObject rootObj = doc.object();
        QJsonValue defaultP =rootObj.value("defaultP");
        QJsonValue url = rootObj.value("inputUrl");

        qDebug()<<"url:"<<url.toString() <<endl;
        qDebug()<<"defaultP:"<<defaultP.toString() <<endl;
        int defaultPIndex=-1;
        if(defaultP.toString()=="1080p"){
            defaultPIndex=0;
        }else if(defaultP.toString()=="720p"){
            defaultPIndex=1;
        }else if(defaultP.toString()=="480p"){
            defaultPIndex=2;
        }else if(defaultP.toString()=="360p"){
            defaultPIndex=3;
        }
        int findResult=url.toString().indexOf("https://www.bilibili.com/video/");
        //assert

        if(findResult!=0){

            qDebug()<<"web url not suppport"<<endl;

            //addVideoFile(s);
            //mPlayer->startPlay(s.toStdString());
           // return ;
            response.setHeader("Content-Type", "text/plain; charset=UTF-8");
            array+="web url not suppport";
            response.write(array,true);
        }
        else if(defaultPIndex==-1){
            qDebug()<<"default p not suppport"<<endl;
            response.setHeader("Content-Type", "text/plain; charset=UTF-8");
            array+="default p not suppport";
            response.write(array,true);
        }
        else{
            setDownloadingFlag(true);
            if(!newThread->isRunning())
                newThread->start();

            qDebug()<<"web thread Downloading"<<endl;

        emit add_url(QString(url.toString()),defaultPIndex);

            response.setHeader("Content-Type", "text/plain; charset=UTF-8");
            array+="web thread Downloading";
            response.write(array,true);
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


void DownloadController::close_download_web_thread(){
    if(newThread->isRunning())
       {
           newThread->quit();
           newThread->wait();
       }

    setDownloadingFlag(false);

      qDebug()<<QThread::currentThreadId()<<"recv work stop signal"<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}


void DownloadController::setDownloadingFlag(BOOL flag){
    mDownloadingFlag=flag;
}

BOOL DownloadController::getDownloadingFlag(){
   return mDownloadingFlag;
}

void DownloadController::slot_web_download_dequeue(DownloadItem mDownloadItem){
    mDownloadResult=mDownloadItem;
    qDebug()<<mDownloadItem.url<<endl;
    qDebug()<<mDownloadItem.defaultP<<endl;
    qDebug()<<mDownloadItem.downloadFlag<<endl;
    qDebug()<<mDownloadItem.downloadFilepath<<endl;
    if(mDownloadItem.downloadFlag == SUCCESS)
        emit add_file(mDownloadResult.downloadFilepath);
}
