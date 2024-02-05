/**
  @file
  @author Stefan Frings
*/

#include "listcontroller.h"
#include <QVariant>
#include <QDateTime>
#include <QThread>
#include <QFile>
#include <QFileInfo>
#include <QJsonObject> // { }
#include <QJsonArray> // [ ]
#include <QJsonDocument> // 解析Json
#include <QJsonValue> // int float double bool null { } [ ]
#include <QJsonParseError>

extern QString docrootPath;

ListController::ListController()
{
    requestFlag=1;
}

void ListController::moveToNext(HttpRequest& request, HttpResponse& response)
{
    QString filename="index.html";
    QByteArray array="";
    QFile file(docrootPath+filename);
    QByteArray htmlBody=request.getBody();
    int item=htmlBody.toInt();

    qDebug()<<"sig_moveToNext:"<< item<<endl;
    emit sig_moveToNext(item);


    bool isok = file.open(QIODevice::ReadOnly); //只读模式打开
    if(isok == true){
        array  =  file.readAll();
        file.close();
    }
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    response.write(array,true);
}


void ListController::service(HttpRequest& request, HttpResponse& response)
{
    QString filename="index.html";
    QByteArray array="";

    if(requestFlag==2){
        response.setHeader("Content-Type", "text/plain; charset=UTF-8");
        QJsonDocument document;
        QJsonObject jsonObject;

        for(int i=0;i<mVideoList->size();i++){
            jsonObject.insert(QString(i),mVideoList->at(i));
        }


        document.setObject(jsonObject);
        QByteArray simpbyte_array = document.toJson(QJsonDocument::Compact);
        QString simpjson_str(simpbyte_array);

        qDebug()<<"QJsonValue:"<<simpjson_str;
        qDebug()<<"mCurrentIndex:"<<*mCurrentIndexPtr;
        array+="mVideoList";
        response.write(array,true);
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

void ListController::slot_send_list(QStringList* mList,int* mIndex){
    requestFlag=2;
    mVideoList=mList;
    mCurrentIndexPtr=mIndex;
}
