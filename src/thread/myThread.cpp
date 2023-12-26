#include "myThread.h"
#include <QDebug>
#include <QString>
#include <qdatetime.h>
#include <windows.h>
#include <QFile>
#include <QFileDialog>
#include <QThread>

myThread::myThread(QObject *parent) : QObject(parent)
{
    connect(this,SIGNAL(download_start()),this,SLOT(downloadCmd()));
}
void myThread::display()
{
    quint16 a = 0;
    while(!stopFlag)
    {
        qDebug()<<"a = "<<a;
        a++;
    }
}
void myThread::setFlag(bool flag)
{
    stopFlag = flag;
}

void myThread::setPath(QString folderPath)
{
    FolderPath=folderPath;
}

void myThread::addurl(QString s){
    qDebug() << "addurl " << endl;
    q.enqueue(s);

    if(!downloadingFlag){
        downloadingFlag=true;
        //downloadCmd();
//        QThread::msleep(5000);
//        qDebug() << "thread exit" << endl;
//        downloadingFlag=false;
        emit download_start();
    }
}

void myThread::downloadCmd(){

    while(!q.isEmpty())
    {
        qDebug() << " thread downloading " << endl;
    QString s=q.dequeue();
    QString strTime = QDateTime::currentDateTime().toString("hh-mm-ss");//格式化时间
    QString DirPath= FolderPath+"/D-"+strTime;
    QDir dir(DirPath);
    DirPath = dir.fromNativeSeparators(DirPath);//  "\\"转为"/"

     if (!dir.exists()) dir.mkdir(DirPath);

    QString downloadCmdFilePath(DirPath+QString("/download.cmd"));
    QFile downloadCmdFile(downloadCmdFilePath);

    downloadCmdFile.open(QIODevice::ReadWrite);
    //downloadCmdFile.write("@echo off");
    //downloadCmdFile.write("\nlux.exe -c bilibili_cookie.txt");
    downloadCmdFile.write("\nlux.exe ");
    downloadCmdFile.write(" -o Downloads/D-");
    downloadCmdFile.write(strTime.toStdString().c_str());
    downloadCmdFile.write(" ");
    //downloadCmdFile.write(" -f 80-7 ");
    downloadCmdFile.write(s.toStdString().c_str());

    downloadCmdFile.write("\ncd Downloads/D-");
    downloadCmdFile.write(strTime.toStdString().c_str());
    downloadCmdFile.write("\ndir /b *.mp4> name1.txt");
    downloadCmdFile.write("\ndir /b *.m4a> name2.txt");
    downloadCmdFile.write("\nset /p filename1=<name1.txt");
    downloadCmdFile.write("\nset /p filename2=<name2.txt");
    //downloadCmdFile.write("\ndel /F name1.txt");
    //downloadCmdFile.write("\ndel /F name2.txt");

    downloadCmdFile.write("\nren \"%filename1%\" \"merge1.mp4\"");
    downloadCmdFile.write("\nren \"%filename2%\" \"merge2.m4a\"");

    downloadCmdFile.write("\ncd ..");
    downloadCmdFile.write("\ncd ..");
    downloadCmdFile.write("\nffmpeg -i \"");
    downloadCmdFile.write(DirPath.toStdString().c_str());
    downloadCmdFile.write("/merge1.mp4\" -i \"");
    downloadCmdFile.write(DirPath.toStdString().c_str());
    downloadCmdFile.write("/merge2.m4a\" -y -acodec copy -vcodec copy ");
    downloadCmdFile.write((DirPath+"/mergeResult.mp4").toStdString().c_str());
    downloadCmdFile.write("\ncd Downloads/D-");
    downloadCmdFile.write(strTime.toStdString().c_str());

    downloadCmdFile.write("\nren \"mergeResult.mp4\" \"%filename1%\"");
    downloadCmdFile.write("\nmove \"%filename1%\" ../");
    //downloadCmdFile.write("\ncd D-");
    //downloadCmdFile.write(strTime.toStdString().c_str());

    //downloadCmdFile.write("\ndel /F merge1.mp4");
    //downloadCmdFile.write("\ndel /F merge2.m4a");
    //downloadCmdFile.write("\ndel /F download.cmd");

    downloadCmdFile.close();

    STARTUPINFO si;//创建进程所需的信息结构变量
        GetStartupInfo(&si);
        si.lpReserved = NULL;
        si.lpDesktop = NULL;
        si.lpTitle = NULL;
        si.dwX = 0;
        si.dwY = 0;
        si.dwXSize = 0;
        si.dwYSize = 0;
        si.dwXCountChars = 500;
        si.dwYCountChars = 500;
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_SHOW;
        //说明进程将以隐藏的方式在后台执行
        si.cbReserved2 = 0;
        si.lpReserved2 = NULL;
        si.hStdInput = stdin;
        si.hStdOutput = stdout;
        si.hStdError = stderr;
        PROCESS_INFORMATION pi;
        std::wstring strCmdLine = downloadCmdFilePath.toStdWString();

        DWORD dwExitCode;
        DWORD t1 = GetTickCount();
        if (CreateProcess(NULL, (LPWSTR)strCmdLine.c_str(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
        {
            WaitForSingleObject(pi.hProcess, 300*1000);
            DWORD t2 = GetTickCount();
            if (GetExitCodeProcess(pi.hProcess, &dwExitCode))
            {
                 qDebug() << dwExitCode << endl;
                if (dwExitCode == 0)
                {
                    qDebug() << "CreateProcess Exit sucess" << endl;
                }
                else
                {
                    qDebug() << "CreateProcess Exit failed" << endl;
                    //emit fail

                }
                dir.removeRecursively();
            }
             qDebug() << "convert consume " << t2 - t1 << "ms" << endl;
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
            if(q.isEmpty()){
                qDebug() << "thread exit" << endl;
                downloadingFlag=false;
                emit sig_downloadCmd_finished();
                break;
            }
        }
        else
        {
             qDebug() << "Create Fail" << endl;
        }

    }





}




