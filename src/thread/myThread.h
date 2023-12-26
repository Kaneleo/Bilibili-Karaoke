#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QString.h>
#include <QQueue>
extern int counter;
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
    QQueue<QString> q;
signals:
    void download_start();
    void sig_downloadCmd_finished();
public slots:
    void setFlag(bool flag = false);
    void display();
    void addurl(QString s);
    void downloadCmd();

};

#endif // MYTHREAD_H

