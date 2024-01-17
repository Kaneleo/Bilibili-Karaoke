#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QString.h>
#include <QQueue>
extern int counter;

enum DOWNLOAD_FLAG{
    WAIT=0,
    DOWNLOADING,
    SUCCESS,
    FAILED
};

typedef struct
{
    QString url;
    int defaultP;
    DOWNLOAD_FLAG downloadFlag;
    QString downloadFilepath;
}DownloadItem;

class myThread : public QObject
{
    Q_OBJECT
public:
    explicit myThread(QObject *parent = 0);
    bool stopFlag = false;
    bool downloadingFlag = false;
    void setPath(QString folderPath);

private:
    QString FolderPath;
    QQueue<DownloadItem> q;
signals:
    void download_start();
    void sig_downloadCmd_finished();
    void sig_download_dequeue(DownloadItem mDownloadItem);
public slots:
    void setFlag(bool flag = false);
    void display();
    void addurl(QString s,int defaultP);
    void downloadCmd();


};

#endif // MYTHREAD_H

