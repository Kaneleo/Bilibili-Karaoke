﻿/**
 * 叶海辉
 * QQ群121376426
 * http://blog.yundiantech.com/
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QListWidgetItem>

#include <QImage>
#include <QPaintEvent>
#include <QTimer>
#include <QPushButton>
#include <QPropertyAnimation>

#include <QMenu>
#include <QAction>

#include "VideoPlayer/VideoPlayer.h"
#include "DragAbleWidget.h"
#include "thread/myThread.h"

namespace Ui {
class MainWindow;
}

///这个是播放器的主界面 包括那些按钮和进度条之类的
class MainWindow : public DragAbleWidget, public VideoPlayerCallBack
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addVideoFiles(const QStringList &videoFileList);
    void addVideoFile(const QString &filePath);
    void clear();

    void startPlay(); //播放第一个
    void stopPlay();
    void setFolderPath(const QString FolderPath);
    void setDownloadThread(myThread *myObject1);
    void setDownloadP(int defaultP);
    BOOL getDownloadingFlag();

protected:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *target, QEvent *event);

private:
    Ui::MainWindow *ui;

    int mDefaultP=0;

    DownloadItem mDownloadResult;
    BOOL mDownloadingFlag;

    myThread *myObject;
    QThread *newThread;

    QString mFolderPath;

    VideoPlayer *mPlayer; //播放线程
    QTimer *mTimer; //定时器-获取当前视频时间
    float mVolume=100;

    QTimer *mCheckFilesTimer;
    QTimer *mChangeTimer;

    QTimer *mTimer_CheckControlWidget; //用于控制控制界面的出现和隐藏
    QPropertyAnimation *mAnimation_ControlWidget;   //控制底部控制控件的出现和隐藏
    void showOutControlWidget(); //显示底部控制控件
    void hideControlWidget();    //隐藏底部控制控件

    QMenu* mPopMenu;

    QAction *mAddVideoAction;
    QAction *mEditVideoAction;
    QAction *mDeleteVideoAction;
    QAction *mClearVideoAction;
    QAction *mChangeVocalAction;
    QAction *mSwitchToNextAction;
    QAction *mMoveToNextAction;

    void setDownloadingFlag(BOOL flag);

    void doAdd();
    void doAddStream();
    void doAddUrl(QString s,int defaultp);
    void doDelete();
    bool doDeleteFile(const QString &strPath);
    void doClear();
    void doChangeVocal();
    void doSwitchToNext();
    void doMoveToNext();

    void setVideoNums(const int &nums);


    QStringList mVideoFileList;
    QStringList mVideoFileListLocal;


    int mCurrentIndex;
    QListWidgetItem *mCurrentItem;
    bool mIsNeedPlayNext;

    void playVideo(const int &index);
    void playVideoFile(const QString &filePath);
signals:
        void sig_download(QString s,int dp);
        void sig_send_list(QStringList* mList, int* mIndex);

private slots:
    ///播放器相关的槽函数
    void slotSliderMoved(int value);
    void slotTimerTimeOut();
    void slotBtnClick(bool isChecked);
    void slotItemDoubleClicked(QListWidgetItem *item);

    void slotCustomContextMenuRequested();
    void slotActionClick();
    void close_downloadCmd();

    void slotSetP(int PIndex);
    void addFileFromWeb(QString filepath);

    void web_resume_slot();
    void web_next_slot();
    void web_mute_slot();
    void slot_download_dequeue(DownloadItem mDownloadItem);
    void moveToNext(int selectItem);

    ///以下函数，是播放器的回调函数，用于输出信息给界面
protected:
    ///打开文件失败
    void onOpenVideoFileFailed(const int &code);

    ///打开sdl失败的时候回调此函数
    void onOpenSdlFailed(const int &code);

    ///获取到视频时长的时候调用此函数
    void onTotalTimeChanged(const int64_t &uSec);

    ///播放器状态改变的时候回调此函数
    void onPlayerStateChanged(const VideoPlayerState &state, const bool &hasVideo, const bool &hasAudio);

    ///显示视频数据，此函数不宜做耗时操作，否则会影响播放的流畅性。
    void onDisplayVideo(VideoFramePtr videoFrame);

};

#endif // MAINWINDOW_H
