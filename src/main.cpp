
/**
 * 叶海辉
 * QQ群121376426
 * http://blog.yundiantech.com/
 */

#include <QApplication>
#include <QTextCodec>

#include <QDebug>

#include "AppConfig.h"
#include "MainWindow.h"

#include <QDir>
#include "src/Web/global.h".h"
#include "httplistener.h"
#include "src/Web/requestmapper.h"
#include "src/Web/Controller/DownloadController.h"
#include "src/thread/myThread.h"


using namespace stefanfrings;
QString searchConfigFile();

QString docrootPath;
RequestMapper *newRequestMapper;

#undef main
int main(int argc, char *argv[])
{
//    bool userSoftWareOpenGL = false; //使用软件模拟openGL

//    for (int i=0;i<argc;i++)
//    {
//        QString str = QString(argv[i]);

//        if (str == "usesoftopengl")
//        {
//            userSoftWareOpenGL = true;
//        }
//        else if (str == "harddecoder")
//        {
//            AppConfig::gVideoHardDecoder = true;
//        }
//        qDebug()<<__FUNCTION__<<argv[i]<<str;
//    }

//    if (userSoftWareOpenGL)
//    {
//        qDebug()<<__FUNCTION__<<"\n\n !!! userSoftWareOpenGL !!! \n\n";
//        ///没有安装显卡驱动的系统需要使用软件模拟的openGL，一般是linux系统下会存在这种情况
//        QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
//    }
//    else
//    {
//#if defined(WIN32)
//        QCoreApplication::setAttribute(Qt::AA_UseOpenGLES); //解决windows下使用独立显卡的时候 切换全屏 闪烁问题
//#else

//#endif
//    }

    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    AppConfig::InitAllDataPath(); //初始化一些变量和路径信息
    //AppConfig::loadConfigInfoFromFile();

    //AppConfig::WriteLog(QString( "\n #############\n Version = %1 \n ##############").arg(AppConfig::VERSION_NAME));

       a.setApplicationName("Web");
    QString configFileName=searchConfigFile();

    // Configure static file controller
    QSettings* fileSettings=new QSettings(configFileName,QSettings::IniFormat,&a);
    fileSettings->beginGroup("docroot");
    staticFileController=new StaticFileController(fileSettings,&a);

    // Configure and start the TCP listener
    QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat,&a);
    listenerSettings->beginGroup("listener");
    newRequestMapper = new RequestMapper(&a);
    new HttpListener(listenerSettings,newRequestMapper,&a);
    MainWindow w;



//    w.setDownloadThread(myObject);
//    newThread->start();
    w.show();

    return a.exec();
}


/** Search the configuration file */
QString searchConfigFile()
{
    QString binDir=QCoreApplication::applicationDirPath();
    docrootPath=QCoreApplication::applicationDirPath()+"/../src/Web/doc/docroot/";
    QString appName=QCoreApplication::applicationName();
    QString fileName("Demo1.ini");

    QStringList searchList;
    searchList.append(binDir);
    searchList.append(binDir+"/etc");
    searchList.append(binDir+"/../etc");
    searchList.append(binDir+"/../src/Web/doc");
    searchList.append(binDir+"/../"+appName+"/etc");     // for development with shadow build (Linux)
    searchList.append(binDir+"/../../"+appName+"/etc");  // for development with shadow build (Windows)
    searchList.append(binDir+"/../../etc");
    searchList.append(QDir::rootPath()+"etc/opt");
    searchList.append(QDir::rootPath()+"etc");

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }
    qFatal("Cannot find config file %s",qPrintable(fileName));
    return nullptr;
}


