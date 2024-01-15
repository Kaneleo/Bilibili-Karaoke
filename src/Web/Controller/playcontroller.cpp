/**
  @file
  @author Stefan Frings
*/

#include "playcontroller.h"
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

PlayController::PlayController()
{}

void PlayController::resumeService(HttpRequest& request, HttpResponse& response){
    QString filename="index.html";
    QByteArray array="";
    QFile file(docrootPath+filename);

    emit web_resume_sig();
    qDebug()<<"web_resume_sig"<<endl;

    bool isok = file.open(QIODevice::ReadOnly); //只读模式打开
    if(isok == true){
        array  =  file.readAll();
        file.close();
    }
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    response.write(array,true);
}

void PlayController::nextService(HttpRequest& request, HttpResponse& response){
    QString filename="index.html";
    QByteArray array="";
    QFile file(docrootPath+filename);

    emit web_next_sig();
    qDebug()<<"web_next_sig"<<endl;

    bool isok = file.open(QIODevice::ReadOnly); //只读模式打开
    if(isok == true){
        array  =  file.readAll();
        file.close();
    }
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    response.write(array,true);
}

void PlayController::muteService(HttpRequest& request, HttpResponse& response)
{
    QString filename="index.html";
    QByteArray array="";
    QFile file(docrootPath+filename);

    emit web_mute_sig();
    qDebug()<<"web_mute_sig"<<endl;

    bool isok = file.open(QIODevice::ReadOnly); //只读模式打开
    if(isok == true){
        array  =  file.readAll();
        file.close();
    }
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    response.write(array,true);
}
