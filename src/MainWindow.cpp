/**
 * 叶海辉
 * QQ群121376426
 * http://blog.yundiantech.com/
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QPainter>
#include <QPaintEvent>
#include <QFileDialog>
#include <QDebug>
#include <QDesktopWidget>
#include <QFontDatabase>
#include <QMouseEvent>
#include <QMessageBox>

#include "AppConfig.h"
#include "Base/FunctionTransfer.h"

#include "Widget/SetVideoUrlDialog.h"
#include "Widget/mymessagebox_withTitle.h"

#include <windows.h>
#include <QFile>
#include "src/Web/Controller/downloadcontroller.h"
#include <QFileDialog>
#include "src/Web/requestmapper.h"

extern RequestMapper *newRequestMapper;


Q_DECLARE_METATYPE(VideoPlayerState)

MainWindow::MainWindow(QWidget *parent) :
    DragAbleWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this->getContainWidget());


    newThread = new QThread;
    myObject = new myThread;
    myObject->setPath(QCoreApplication::applicationDirPath()+"/Downloads");
    myObject->moveToThread(newThread);

    FunctionTransfer::init(QThread::currentThreadId());

    ///初始化播放器
    VideoPlayer::initPlayer();

    setWindowFlags(Qt::FramelessWindowHint);//|Qt::WindowStaysOnTopHint);  //使窗口的标题栏隐藏
//    setAttribute(Qt::WA_TranslucentBackground);

    //因为VideoPlayer::PlayerState是自定义的类型 要跨线程传递需要先注册一下
    qRegisterMetaType<VideoPlayerState>();

    mPopMenu = new QMenu(this);

    mAddVideoAction              = new QAction(QIcon("images/open.png"), QStringLiteral("打开网络流"), this);
    mEditVideoAction             = new QAction(QIcon("images/open.png"), QStringLiteral("修改数据"), this);
    mDeleteVideoAction           = new QAction(QIcon("images/open.png"), QStringLiteral("删除"), this);
    mClearVideoAction            = new QAction(QIcon("images/open.png"), QStringLiteral("清空"), this);
    mChangeVocalAction           = new QAction(QIcon("images/open.png"), QStringLiteral("切换原唱伴唱"), this);
    mSwitchToNextAction           = new QAction(QIcon("images/open.png"), QStringLiteral("下一首"), this);
    mPopMenu->addAction(mAddVideoAction);
//    mPopMenu->addAction(mEditVideoAction);
//    mPopMenu->addSeparator();       //添加分离器
    mPopMenu->addAction(mDeleteVideoAction);
    mPopMenu->addAction(mClearVideoAction);
    mPopMenu->addAction(mChangeVocalAction);
    mPopMenu->addAction(mSwitchToNextAction);

    connect(this,SIGNAL(sig_download(QString,int)),myObject,SLOT(addurl(QString,int)),Qt::DirectConnection);
    connect(myObject,SIGNAL(sig_downloadCmd_finished(QString)),this,SLOT(close_downloadCmd(QString)));
    connect(newRequestMapper->mDownloadController,SIGNAL(add_file(QString)),this,SLOT(addFileFromWeb(QString)));

    connect(newRequestMapper->mPlayController,SIGNAL(web_resume_sig()),this,SLOT(web_resume_slot()));
    connect(newRequestMapper->mPlayController,SIGNAL(web_next_sig()),this,SLOT(web_next_slot()));
    connect(newRequestMapper->mPlayController,SIGNAL(web_mute_sig()),this,SLOT(web_mute_slot()));

    connect(mAddVideoAction,     &QAction::triggered, this, &MainWindow::slotActionClick);
    //connect(mEditVideoAction,    &QAction::triggered, this, &MainWindow::slotActionClick);
    connect(mDeleteVideoAction,  &QAction::triggered, this, &MainWindow::slotActionClick);
    connect(mClearVideoAction,   &QAction::triggered, this, &MainWindow::slotActionClick);
    connect(mChangeVocalAction,  &QAction::triggered, this, &MainWindow::slotActionClick);
    connect(mSwitchToNextAction,  &QAction::triggered, this, &MainWindow::slotActionClick);

    connect(ui->pushButton_open, &QPushButton::clicked, this, &MainWindow::slotBtnClick);
    connect(ui->toolButton_open, &QPushButton::clicked, this, &MainWindow::slotBtnClick);
    connect(ui->pushButton_clear,&QPushButton::clicked, this, &MainWindow::slotBtnClick);
    connect(ui->pushButton_play, &QPushButton::clicked, this, &MainWindow::slotBtnClick);
    connect(ui->pushButton_pause,&QPushButton::clicked, this, &MainWindow::slotBtnClick);
    connect(ui->pushButton_stop, &QPushButton::clicked, this, &MainWindow::slotBtnClick);
    connect(ui->pushButton_volume, &QPushButton::clicked, this, &MainWindow::slotBtnClick);



    connect(ui->horizontalSlider, SIGNAL(sig_valueChanged(int)), this, SLOT(slotSliderMoved(int)));
    connect(ui->horizontalSlider_volume, SIGNAL(valueChanged(int)), this, SLOT(slotSliderMoved(int)));

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::slotItemDoubleClicked);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::slotCustomContextMenuRequested);

    ui->page_video->setMouseTracking(true);
    ui->widget_videoPlayer->setMouseTracking(true);
//    ui->page_video->installEventFilter(this);
    ui->widget_videoPlayer->installEventFilter(this);
    ui->widget_container->installEventFilter(this);

    mCurrentIndex = 0;
    mCurrentItem = nullptr;
    mIsNeedPlayNext = false;

    mPlayer = new VideoPlayer();
    mPlayer->setVideoPlayerCallBack(this);

    mTimer = new QTimer; //定时器-获取当前视频时间
    connect(mTimer, &QTimer::timeout, this, &MainWindow::slotTimerTimeOut);
    mTimer->setInterval(500);


//    mCheckFilesTimer = new QTimer;
//    connect(mCheckFilesTimer, &QTimer::timeout, this, &MainWindow::slotTimerTimeOut);
//    setFolderPath(QDir::currentPath()+"/Downloads");
//    mCheckFilesTimer->start(500);

//    mChangeTimer = new QTimer;
//    connect(mChangeTimer, &QTimer::timeout, this, &MainWindow::slotTimerTimeOut);
    //mChangeTimer->start(5000);

    mTimer_CheckControlWidget = new QTimer; //用于控制控制界面的出现和隐藏
    connect(mTimer_CheckControlWidget, &QTimer::timeout, this, &MainWindow::slotTimerTimeOut);
    mTimer_CheckControlWidget->setInterval(1000);

    mAnimation_ControlWidget  = new QPropertyAnimation(ui->widget_controller, "geometry");

    ui->stackedWidget->setCurrentWidget(ui->page_open);
    ui->pushButton_pause->hide();

    resize(1024,768);
    setTitle(QStringLiteral("我的播放器-V%1").arg(AppConfig::VERSION_NAME));

    mVolume = mPlayer->getVolume();

    mDownloadingFlag=false;



    std::thread([=]
    {
        while (1)
        {

            AppConfig::mSleep(500);

            static QPoint lastPoint = QPoint(0, 0);

            FunctionTransfer::runInMainThread([=]()
            {
                QPoint point = QCursor::pos();

                if (this->geometry().contains(point))
                {
                    if (lastPoint != point)
                    {
                        if (!mTimer_CheckControlWidget->isActive())
                        {
                            showOutControlWidget();
                        }

                        mTimer_CheckControlWidget->stop();
                        mTimer_CheckControlWidget->start();

                        lastPoint = point;
                    }
                }
            });
        }

    }).detach();
}

MainWindow::~MainWindow()
{
qDebug()<<__FUNCTION__;

    //AppConfig::saveConfigInfoToFile();
    AppConfig::removeDirectory(AppConfig::AppDataPath_Tmp);

    delete ui;
}

void MainWindow::showOutControlWidget()
{

    mAnimation_ControlWidget->setDuration(800);

    int w = ui->widget_controller->width();
    int h = ui->widget_controller->height();
    int x = 0;
    int y = ui->widget_container->height() - ui->widget_controller->height();

    if (ui->widget_controller->isHidden())
    {
        ui->widget_controller->show();
        mAnimation_ControlWidget->setStartValue(ui->widget_controller->geometry());
    }
    else
    {
        mAnimation_ControlWidget->setStartValue(ui->widget_controller->geometry());
    }

//    mAnimation_ControlWidget->setKeyValueAt(0, QRect(0, 0, 00, 00));
//    mAnimation_ControlWidget->setKeyValueAt(0.4, QRect(20, 250, 20, 30));
//    mAnimation_ControlWidget->setKeyValueAt(0.8, QRect(100, 250, 20, 30));
//    mAnimation_ControlWidget->setKeyValueAt(1, QRect(250, 250, 100, 30));
    mAnimation_ControlWidget->setEndValue(QRect(x, y, w, h));
    mAnimation_ControlWidget->setEasingCurve(QEasingCurve::Linear); //设置动画效果

    mAnimation_ControlWidget->start();

}

void MainWindow::hideControlWidget()
{
    mAnimation_ControlWidget->setTargetObject(ui->widget_controller);

    mAnimation_ControlWidget->setDuration(300);

    int w = ui->widget_controller->width();
    int h = ui->widget_controller->height();
    int x = 0;
    int y = ui->widget_container->height() + h;

    mAnimation_ControlWidget->setStartValue(ui->widget_controller->geometry());
    mAnimation_ControlWidget->setEndValue(QRect(x, y, w, h));
    mAnimation_ControlWidget->setEasingCurve(QEasingCurve::Linear); //设置动画效果

    mAnimation_ControlWidget->start();
}


void MainWindow::setVideoNums(const int &nums)
{
    ui->label_num->setText(QStringLiteral("%1个文件").arg(nums));
}

void MainWindow::addVideoFiles(const QStringList &videoFileList)
{
    if (!videoFileList.isEmpty())
    {
        QString filePath = videoFileList.first();
        AppConfig::gVideoFilePath = QFileInfo(filePath).absoluteDir().path();
        //AppConfig::saveConfigInfoToFile();
    }

    for (QString filePath : videoFileList)
    {
        addVideoFile(filePath);
    }
}

void MainWindow::addVideoFile(const QString &filePath)
{
    QFileInfo fileInfo(filePath);

    QListWidgetItem *item = new QListWidgetItem();
    item->setText(fileInfo.fileName());
    ui->listWidget->addItem(item);

    mVideoFileList.append(filePath);

    setVideoNums(mVideoFileList.size());
}

void MainWindow::clear()
{
    stopPlay();
    ui->listWidget->clear();
    doDeleteFile(mFolderPath);
    mVideoFileList.clear();
    mVideoFileListLocal.clear();

    setVideoNums(mVideoFileList.size());
}

void MainWindow::startPlay()
{
    playVideo(0);
}

void MainWindow::setFolderPath(const QString FolderPath)
{
    mFolderPath=FolderPath;
}

void MainWindow::setDownloadThread(myThread *myObject1){
    myObject=myObject1;
}

void MainWindow::setDownloadP(int defaultP){
    mDefaultP=defaultP;
}

void MainWindow::stopPlay()
{
    if (mCurrentItem != nullptr)
    {
        mCurrentItem->setBackgroundColor(QColor(0, 0, 0, 0));
    }

    mCurrentItem = nullptr;

    mIsNeedPlayNext = false;
    mPlayer->stop(true);
}

void MainWindow::playVideo(const int &index)
{
    int playIndex = index;

//    ///播放到最后一个后，从头开始播放
//    {
//        if ((playIndex < 0) || (mVideoFileList.size() <= playIndex))
//        {
//            playIndex = 0;
//        }
//    }

    if (index >= 0 && mVideoFileList.size() > playIndex)
    {
        mCurrentIndex = playIndex;

        QString filePath = mVideoFileList.at(playIndex);

qDebug()<<__FUNCTION__<<filePath<<playIndex;

        playVideoFile(filePath);

//        ui->listWidget->setCurrentRow(playIndex);
        QListWidgetItem *item = ui->listWidget->item(playIndex);

        if (mCurrentItem != nullptr)
        {
            mCurrentItem->setBackgroundColor(QColor(0, 0, 0, 0));
        }

        mCurrentItem = item;

        mCurrentItem->setBackgroundColor(QColor(75, 92, 196));
    }
}

void MainWindow::playVideoFile(const QString &filePath)
{
    qDebug()<<"playVideoFile"<<endl;
    mIsNeedPlayNext = false;
    //mPlayer->stop(true);

    mPlayer->startPlay(filePath.toStdString());

}

void MainWindow::slotSliderMoved(int value)
{
    if (QObject::sender() == ui->horizontalSlider)
    {
        mPlayer->seek((qint64)value * 1000000);
    }
    else if (QObject::sender() == ui->horizontalSlider_volume)
    {
        mPlayer->setVolume(value / 100.0);
        ui->label_volume->setText(QString("%1").arg(value));
    }
}

void MainWindow::slotTimerTimeOut()
{
    if (QObject::sender() == mTimer)
    {
        qint64 Sec = mPlayer->getCurrentTime();

        ui->horizontalSlider->setValue(Sec);

        QString curTime;
        QString hStr = QString("0%1").arg(Sec / 3600);
        QString mStr = QString("0%1").arg(Sec / 60 % 60);
        QString sStr = QString("0%1").arg(Sec % 60);
        if (hStr == "00")
        {
            curTime = QString("%1:%2").arg(mStr.right(2)).arg(sStr.right(2));
        }
        else
        {
            curTime = QString("%1:%2:%3").arg(hStr).arg(mStr.right(2)).arg(sStr.right(2));
        }

        ui->label_currenttime->setText(curTime);
    }
    else if (QObject::sender() == mTimer_CheckControlWidget)
    {
        //ylw checklist

        mTimer_CheckControlWidget->stop();
        hideControlWidget();
    }

    else if (QObject::sender() == mCheckFilesTimer){

//        if (!mFolderPath.isEmpty()){
//            // 获取所有文件名
//            QStringList fileList;
//            QDir dir(mFolderPath);
//            QStringList mImgNames;
//            QString newFileName;
//            mFolderPath = dir.fromNativeSeparators(mFolderPath);//  "\\"转为"/"
//            if (!dir.exists()) dir.mkdir(mFolderPath);
//            dir.setFilter(QDir::Files);
//            dir.setSorting(QDir::Name);
//            dir.setNameFilters(QString("*.mp4").split(";"));
//            mImgNames = dir.entryList();

//            if(mImgNames.size()>mVideoFileListLocal.size()){
//                //添加文件
//                for (int i = 0; i < mImgNames.size(); ++i)
//                {
//                     newFileName= mFolderPath + "/" +mImgNames[i];
//                     if(mVideoFileListLocal.indexOf(newFileName)==-1){
//                         fileList.append(newFileName);
//                         mVideoFileListLocal.append(newFileName);
//                     }
//                }
//                addVideoFiles(fileList);
//            }
//            else if(mImgNames.size()<mVideoFileListLocal.size()){
//                for (int j = 0; j < mVideoFileListLocal.size(); ++j)
//                {
//                     newFileName= mVideoFileListLocal.at(j);
//                     QFileInfo fileI2nfo(newFileName);
//                     if(mImgNames.indexOf(fileI2nfo.fileName())==-1){

//                         //mVideoFileList.removeAt(j);
//                         mVideoFileListLocal.removeAt(j);

//                         for(int mVideoFileListIndex=0; mVideoFileListIndex<mVideoFileList.size();++mVideoFileListIndex){
//                             if(newFileName==mVideoFileList.at(mVideoFileListIndex)){
//                                 mVideoFileList.removeAt(mVideoFileListIndex);
//                                 ui->listWidget->takeItem(mVideoFileListIndex);
//                                 --mVideoFileListIndex;
//                             }
//                         }
//                         break;
//                     }
//                }
//                    setVideoNums(mVideoFileList.size());
//            }
//        }
    }
    else if (QObject::sender() == mChangeTimer){
        mPlayer->setAudioIndex(rand()%3);
    }
}

void MainWindow::setDownloadingFlag(BOOL flag){
    mDownloadingFlag=flag;
}

BOOL MainWindow::getDownloadingFlag(){
   return mDownloadingFlag;
}


void MainWindow::doAdd()
{
    QStringList fileList = QFileDialog::getOpenFileNames(
               this, QStringLiteral("选择要播放的文件"),
                AppConfig::gVideoFilePath,//初始目录
                QStringLiteral("视频文件 (*.flv *.rmvb *.avi *.mp4 *.mkv );;")
                +QStringLiteral("音频文件 (*.mp3 *.wma *.wav *.m4a);;")
                +QStringLiteral("所有文件 (*.*)"));

    if (!fileList.isEmpty())
    {
        addVideoFiles(fileList);
    }

}

void MainWindow::doAddStream()
{
    SetVideoUrlDialog dialog;

//        dialog.setVideoUrl(AppConfig::gVideoFilePath);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString s = dialog.getVideoUrl();
        int defaultp=dialog.getDefaultP();
        //setDownloadP(dialog.getDefaultP());

        doAddUrl(s,defaultp);
    }
}

void MainWindow::doAddUrl(QString s,int defaultp){
            if (!s.isEmpty())
        {
            // mIsNeedPlayNext = false;
            //mPlayer->stop(true); //如果在播放则先停止
 //ylw
            if(s[0]!='h'){
                addVideoFile(s);
                mPlayer->startPlay(s.toStdString());
                return ;
            }
            else{
                if(s.indexOf("https://www.bilibili.com/video/")!=0){

                    qDebug()<<"Selected url is not suppported."<<endl;

                    //addVideoFile(s);
                    //mPlayer->startPlay(s.toStdString());
                   // return ;
                }
                else{
                    setDownloadingFlag(true);
                    if(!newThread->isRunning())
                        newThread->start();
                    qDebug()<<"Downloading"<<endl;
                    emit sig_download(s,defaultp);

                    AppConfig::gVideoFilePath = s;
                    //AppConfig::saveConfigInfoToFile();
                }
            }
        }

}

void MainWindow::doDelete()
{
    QList<int> RowList;

    QList<QListWidgetItem*> selectedItemList = ui->listWidget->selectedItems();

    for (QListWidgetItem* item : selectedItemList)
    {
        int rowValue = ui->listWidget->row(item);

        int index = RowList.size();
        for (int i=0;i<RowList.size();i++)
        {
            int value = RowList.at(i);

            if (rowValue > value)
            {
                index = i;
                break;
            }
        }

        RowList.insert(index, rowValue);
    }
qDebug()<<__FUNCTION__<<RowList;
    if (RowList.isEmpty())
    {
        int ret = MyMessageBox_WithTitle::showWarningText(QStringLiteral("警告"),
                                                           QStringLiteral("请先选择需要删除的数据！"),
                                                           NULL,
                                                           QStringLiteral("关闭"));
        return;
    }

    int ret = MyMessageBox_WithTitle::showWarningText(QStringLiteral("警告"),
                                                       QStringLiteral("确定删除这%1条数据么？").arg(RowList.size()),
                                                       QStringLiteral("确定"),
                                                       QStringLiteral("取消"));

    if (ret == QDialog::Accepted)
    {
        for(int i=0; i<RowList.size(); i++)
        {
            int row = RowList.at(i);

            QString itemToDelete = mVideoFileList.at(row);
            QFileInfo fileI2nfo(itemToDelete);

            mVideoFileList.removeAt(row);

            if(mVideoFileList.indexOf(itemToDelete)==-1){
                if(doDeleteFile(itemToDelete)){
                    qDebug()<<"Del true"<<endl;
                }
                else{
                    mVideoFileList.insert(row,itemToDelete);
                    qDebug()<<"Del false"<<endl;
                    return;
                }
            }

            //mVideoFileList.removeAt(i);

            QListWidgetItem *item = ui->listWidget->takeItem(row);

            if (mCurrentItem == item)
            {
                mCurrentItem = nullptr;
            }

            delete item;

            if (row <= mCurrentIndex)
            {
                mCurrentIndex --;
            }
        }
    }

    setVideoNums(mVideoFileList.size());
}


bool MainWindow::doDeleteFile(const QString &strPath)//要删除的文件夹或文件的路径
{
    if (strPath.isEmpty() || !QDir().exists(strPath))//是否传入了空的路径||路径是否存在
        return false;

    QFileInfo FileInfo(strPath);

    if (FileInfo.isFile())//如果是文件
    {
        if(QFile::remove(strPath)){
            return true;
        }
        else return false;
    }

    else if (FileInfo.isDir())//如果是文件夹
    {
        QDir qDir(strPath);
        if(qDir.removeRecursively()){
            return true;
        }
        else return false;
    }
    return true;
}

void MainWindow::doClear()
{
    int ret = MyMessageBox_WithTitle::showWarningText(QStringLiteral("警告"),
                                                       QStringLiteral("确定要清空所有数据么？"),
                                                       QStringLiteral("确定"),
                                                       QStringLiteral("取消"));

    if (ret == QDialog::Accepted)
    {
        clear();
    }
}

void MainWindow::doChangeVocal(){

    int audioIndex=mPlayer->getAudioIndex();

    if(audioIndex==0)
        mPlayer->setAudioIndex(1);
    else if(audioIndex==1)
        mPlayer->setAudioIndex(0);

}

void MainWindow::doSwitchToNext(){
//    if (mCurrentItem != nullptr)
//    {
//        mCurrentItem->setBackgroundColor(QColor(0, 0, 0, 0));
//    }

    //mCurrentItem = nullptr;

    //mIsNeedPlayNext = false;
    mPlayer->stop(true);

    mPlayer->play();
}




void MainWindow::slotBtnClick(bool isChecked)
{
    if (QObject::sender() == ui->pushButton_play)
    {
        mPlayer->play();
    }
    else if (QObject::sender() == ui->pushButton_pause)
    {
        mPlayer->pause();
    }
    else if (QObject::sender() == ui->pushButton_stop)
    {
        mIsNeedPlayNext = false;
        mPlayer->stop(true);
    }
    else if (QObject::sender() == ui->pushButton_open)
    {
        doAdd();
    }
    else if (QObject::sender() == ui->pushButton_clear)
    {
        doClear();
    }
    else if (QObject::sender() == ui->toolButton_open)
    {
        doAddStream();
    }
    else if (QObject::sender() == ui->pushButton_volume)
    {
       qDebug()<<isChecked;

        bool isMute = isChecked;
        mPlayer->setMute(isMute);

        if (isMute)
        {
            mVolume = mPlayer->getVolume();

            ui->horizontalSlider_volume->setValue(0);
            ui->horizontalSlider_volume->setEnabled(false);
            ui->label_volume->setText(QString("%1").arg(0));
        }
        else
        {
            int volume = mVolume * 100.0;
            ui->horizontalSlider_volume->setValue(volume);
            ui->horizontalSlider_volume->setEnabled(true);
            ui->label_volume->setText(QString("%1").arg(volume));
        }

    }

}


void MainWindow::slotItemDoubleClicked(QListWidgetItem *item)
{
    if (QObject::sender() == ui->listWidget)
    {
        int index = ui->listWidget->row(item);
        stopPlay();

        playVideo(index);
    }
}

void MainWindow::slotCustomContextMenuRequested()
{
//    QPoint point(ui->listWidget->mapFromGlobal(QCursor::pos()));//获取控件的全局坐标

//    int h = ui->tableWidget->horizontalHeader()->height();

//    point.setY(point.y() - h);

//    QTableWidgetItem *item = ui->tableWidget->itemAt(point);

//    if (item == NULL || !item->isSelected())
//    {
//        ui->tableWidget->clearSelection();

//        mAddUserAction->setEnabled(true);
//        mEditUserAction->setEnabled(false);
//        mDeleteUserAction->setEnabled(false);
//        mExportAction->setEnabled(false);
//        mUploadAction_Selected->setEnabled(false);
//    }
//    else
//    {
//        mAddUserAction->setEnabled(false);
//        mEditUserAction->setEnabled(true);
//        mDeleteUserAction->setEnabled(true);
//        mExportAction->setEnabled(true);
//        mUploadAction_Selected->setEnabled(true);
//    }

    mPopMenu->exec(QCursor::pos());
}

void MainWindow::slotActionClick()
{
    if (QObject::sender() == mAddVideoAction)
    {
        doAddStream();
    }
    else if (QObject::sender() == mEditVideoAction)
    {

    }
    else if (QObject::sender() == mDeleteVideoAction)
    {
        doDelete();
    }
    else if (QObject::sender() == mClearVideoAction)
    {
        doClear();
    }
    else if (QObject::sender() == mChangeVocalAction)
    {
        doChangeVocal();
    }
    else if (QObject::sender() == mSwitchToNextAction)
    {
        doSwitchToNext();
    }

}

///打开文件失败
void MainWindow::onOpenVideoFileFailed(const int &code)
{
    FunctionTransfer::runInMainThread([=]()
    {
        QMessageBox::critical(NULL, "tips", QString("open file failed %1").arg(code));
    });
}

///打开SDL失败的时候回调此函数
void MainWindow::onOpenSdlFailed(const int &code)
{
    FunctionTransfer::runInMainThread([=]()
    {
        QMessageBox::critical(NULL, "tips", QString("open Sdl failed %1").arg(code));
    });
}

///获取到视频时长的时候调用此函数
void MainWindow::onTotalTimeChanged(const int64_t &uSec)
{
    FunctionTransfer::runInMainThread([=]()
    {
        qint64 Sec = uSec/1000000;

        ui->horizontalSlider->setRange(0,Sec);

        QString totalTime;
        QString hStr = QString("0%1").arg(Sec/3600);
        QString mStr = QString("0%1").arg(Sec / 60 % 60);
        QString sStr = QString("0%1").arg(Sec % 60);
        if (hStr == "00")
        {
            totalTime = QString("%1:%2").arg(mStr.right(2)).arg(sStr.right(2));
        }
        else
        {
            totalTime = QString("%1:%2:%3").arg(hStr).arg(mStr.right(2)).arg(sStr.right(2));
        }

        ui->label_totaltime->setText(totalTime);
    });
}

///播放器状态改变的时候回调此函数
void MainWindow::onPlayerStateChanged(const VideoPlayerState &state, const bool &hasVideo, const bool &hasAudio)
{
    FunctionTransfer::runInMainThread([=]()
    {
qDebug()<<__FUNCTION__<<state<<mIsNeedPlayNext;
        if (state == VideoPlayer_Stop)
        {
            ui->stackedWidget->setCurrentWidget(ui->page_open);

            ui->pushButton_pause->hide();
            ui->widget_videoPlayer->clear();

            ui->horizontalSlider->setValue(0);
            ui->label_currenttime->setText("00:00");
            ui->label_totaltime->setText("00:00");

            mTimer->stop();

            if (mIsNeedPlayNext)
            {
                mCurrentIndex++;
                playVideo(mCurrentIndex);
            }

            mIsNeedPlayNext = true;
        }
        else if (state == VideoPlayer_Playing)
        {
            if (hasVideo)
            {
                ui->stackedWidget->setCurrentWidget(ui->page_video);
            }
            else
            {
                ui->stackedWidget->setCurrentWidget(ui->page_audio);
            }

            ui->pushButton_play->hide();
            ui->pushButton_pause->show();

            mTimer->start();

            mIsNeedPlayNext = true;
        }
        else if (state == VideoPlayer_Pause)
        {
            ui->pushButton_pause->hide();
            ui->pushButton_play->show();
        }
    });
}

///显示视频数据，此函数不宜做耗时操作，否则会影响播放的流畅性。
void MainWindow::onDisplayVideo(std::shared_ptr<VideoFrame> videoFrame)
{
    ui->widget_videoPlayer->inputOneFrame(videoFrame);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mPlayer->stop(true);
}

//图片显示部件时间过滤器处理
bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->widget_container)
    {
        if(event->type() == QEvent::Resize)
        {
            ///停止动画，防止此时刚好开始动画，导致位置出错
            mAnimation_ControlWidget->stop();

            QResizeEvent * e = (QResizeEvent*)event;
            int w = e->size().width();
            int h = e->size().height();
            ui->widget_video_back->move(0, 0);
            ui->widget_video_back->resize(w, h);

            int x = 0;
            int y = h - ui->widget_controller->height();
            ui->widget_controller->move(x, y);
            ui->widget_controller->resize(w, ui->widget_controller->height());
        }
    }
    else if(target == ui->page_video || target == ui->widget_videoPlayer)
    {
        if(event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress)
        {
            if (!mTimer_CheckControlWidget->isActive())
            {
                showOutControlWidget();
            }

            mTimer_CheckControlWidget->stop();
            mTimer_CheckControlWidget->start();
        }
        else if(event->type() == QEvent::Enter)
        {
            showOutControlWidget();
        }
        else if(event->type() == QEvent::Leave)
        {
            mTimer_CheckControlWidget->stop();
            mTimer_CheckControlWidget->start();
        }
    }

    //其它部件产生的事件则交给基类处理
    return DragAbleWidget::eventFilter(target, event);
}

void MainWindow::close_downloadCmd(QString filepath){
    if(newThread->isRunning())
       {
           newThread->quit();
           newThread->wait();
       }

    setDownloadingFlag(false);
    addVideoFile(filepath);

      qDebug()<<QThread::currentThreadId()<<"recv work stop signal"<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")<<endl;
      qDebug()<<filepath<<endl;
}

void MainWindow::slotSetP(int PIndex){
    mDefaultP = PIndex;
}

void MainWindow::addFileFromWeb(QString filepath){
    addVideoFile(filepath);
}

void MainWindow::web_resume_slot(){
    if (mPlayer->getPlayerState() == VideoPlayer_Playing)
    {
        mPlayer->pause();
    }
    else if(mPlayer->getPlayerState() == VideoPlayer_Pause)
    {
        mPlayer->play();
    }
    else qDebug()<<"web_resume_slot error"<<endl;
}

void MainWindow::web_next_slot(){
    doSwitchToNext();
}

void MainWindow::web_mute_slot(){
    bool ismute=mPlayer->getMute();
    ismute=!ismute;
     mPlayer->setMute(ismute);

     if (ismute)
     {
         mVolume = mPlayer->getVolume();

         ui->horizontalSlider_volume->setValue(0);
         ui->horizontalSlider_volume->setEnabled(false);
         ui->label_volume->setText(QString("%1").arg(0));
     }
     else
     {
         int volume = mVolume * 100.0;
         ui->horizontalSlider_volume->setValue(volume);
         ui->horizontalSlider_volume->setEnabled(true);
         ui->label_volume->setText(QString("%1").arg(volume));
     }
}
