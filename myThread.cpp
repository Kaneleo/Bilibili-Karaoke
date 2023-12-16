#include "myThread.h"
#include <QDebug>
#include <QString>
#include <qdatetime.h>
#include <windows.h>
#include <QFile>
#include <QFileDialog>
myThread::myThread(QObject *parent) : QObject(parent)
{

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

void myThread::downloadCmd(QString s, QString FolderPath){
    QString strTime = QDateTime::currentDateTime().toString("hh-mm-ss");//格式化时间
    QString DirPath= FolderPath+"/D-"+strTime;
    //DirPath.replace("/","\\");
    //DirPath+=strTime;
    QDir dir(DirPath);
    DirPath = dir.fromNativeSeparators(DirPath);//  "\\"转为"/"


     if (!dir.exists()) dir.mkdir(DirPath);

    QString downloadCmdFilePath(DirPath+QString("/download.cmd"));
    QFile downloadCmdFile(downloadCmdFilePath);

    downloadCmdFile.open(QIODevice::ReadWrite);
    downloadCmdFile.write("@echo off");
    downloadCmdFile.write("\nlux.exe -c bilibili_cookie.txt");
    downloadCmdFile.write(" -o Downloads/D-");
    downloadCmdFile.write(strTime.toStdString().c_str());
    downloadCmdFile.write(" -f 80-7 ");
    downloadCmdFile.write(s.toStdString().c_str());

    downloadCmdFile.write("\ncd Downloads/D-");
    downloadCmdFile.write(strTime.toStdString().c_str());
    downloadCmdFile.write("\ndir /b *.mp4> name1.txt");
    downloadCmdFile.write("\ndir /b *.m4a> name2.txt");
    downloadCmdFile.write("\nset /p filename1=<name1.txt");
    downloadCmdFile.write("\nset /p filename2=<name2.txt");
    downloadCmdFile.write("\ndel /F name1.txt");
    downloadCmdFile.write("\ndel /F name2.txt");

    downloadCmdFile.write("\nren \"%filename1%\" \"merge1.mp4\"");
    downloadCmdFile.write("\nren \"%filename2%\" \"merge2.m4a\"");


    downloadCmdFile.write("\ncd ..");
    downloadCmdFile.write("\ncd ..");
//                downloadCmdFile.write("\nmove \"merge1.mp4\" ");
//                downloadCmdFile.write(DirPath.toStdString().c_str());
//                downloadCmdFile.write("\nmove \"merge2.m4a\" ");
//                downloadCmdFile.write(DirPath.toStdString().c_str());

    //downloadCmdFile.write("\ncd ");
    //downloadCmdFile.write(DirPath.toStdString().c_str());
    //downloadCmdFile.write("\nchcp 65001");





    //downloadCmdFile.write("\nmkdir 2=<name2.txt");



//                downloadCmdFile.write("\nren Dir");
//                downloadCmdFile.write(strTime);
//                downloadCmdFile.write("/\"%filename1%\" Dir");
//                downloadCmdFile.write(strTime);
//                downloadCmdFile.write("/\"merge1.mp4\"");

//                downloadCmdFile.write("\nren Dir");
//                downloadCmdFile.write(strTime);
//                downloadCmdFile.write("/\"%filename2%\" Dir");
//                downloadCmdFile.write(strTime);
//                downloadCmdFile.write("/\"merge2.m4a\"");
    //downloadCmdFile.write("\ncd ..");
    //downloadCmdFile.write("\ncd ..");

    downloadCmdFile.write("\nffmpeg -i \"");
    downloadCmdFile.write(DirPath.toStdString().c_str());
    downloadCmdFile.write("/merge1.mp4\" -i \"");
    downloadCmdFile.write(DirPath.toStdString().c_str());
    downloadCmdFile.write("/merge2.m4a\" -y -acodec copy -vcodec copy \"./Downloads/mergeResult.mp4\"");
    downloadCmdFile.write("\ncd Downloads");
    downloadCmdFile.write("\nren \"mergeResult.mp4\" \"%filename1%\"");

    //downloadCmdFile.write("\nmove \"%filename1%\" \"");
    //downloadCmdFile.write(mFolderPath.toStdString().c_str());
    //downloadCmdFile.write("/%filename1%\"");

    //downloadCmdFile.write("\ncd ..");
    downloadCmdFile.write("\ncd D-");
    downloadCmdFile.write(strTime.toStdString().c_str());
    //downloadCmdFile.write("\ncd ..");

    downloadCmdFile.write("\ndel /F merge1.mp4");
    downloadCmdFile.write("\ndel /F merge2.m4a");
    downloadCmdFile.write("\ndel /F download.cmd");

//                downloadCmdFile.write("\nmove \"%filename1%\" \"");
//                downloadCmdFile.write(mFolderPath.toStdString().c_str());
//                downloadCmdFile.write("/%filename1%\"");

    //downloadCmdFile.write("\ncd ..");
    //downloadCmdFile.write("\nrmdir D-");
    //downloadCmdFile.write(strTime.toStdString().c_str());


//                downloadCmdFile.write("\ndel ");

//                downloadCmdFile.write((DirPath+"/download.cmd").toStdString().c_str());
//                downloadCmdFile.write("\ndel ");
//                downloadCmdFile.write((DirPath+"/merge1.mp4").toStdString().c_str());
//                downloadCmdFile.write("\ndel ");
//                downloadCmdFile.write((DirPath+"/merge2.m4a").toStdString().c_str());

//                downloadCmdFile.write("\nrmdir ");
//                downloadCmdFile.write(DirPath.toStdString().c_str());
    //                downloadCmdFile.write("\ndel merge1.mp4");
    //                downloadCmdFile.write("\ndel merge2.m4a");
    //                downloadCmdFile.write("\ndel name1.txt");
    //                downloadCmdFile.write("\ndel name2.txt");
    //  downloadCmdFile.write("\ndel download.cmd");
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
        si.wShowWindow = SW_HIDE;
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
            WaitForSingleObject(pi.hProcess, 500000);
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

                }
                dir.rmdir(DirPath);
            }
             qDebug() << "convert consume " << t2 - t1 << "ms" << endl;
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
        }
        else
        {
             qDebug() << "Create Fail" << endl;
        }

}




