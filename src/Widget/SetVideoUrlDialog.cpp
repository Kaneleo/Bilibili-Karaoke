#include "SetVideoUrlDialog.h"
#include "ui_SetVideoUrlDialog.h"

#include <QFileDialog>

#include "AppConfig.h"
#include <QMenu>
#include <QDebug>

#define MaxP 4

QVector<QString> PDef;
QAction* mParray[MaxP];


SetVideoUrlDialog::SetVideoUrlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetVideoUrlDialog)
{
    ui->setupUi(this);

    QMenu *menuSelection = new QMenu(this);


    PDef.push_back("1080p");
    PDef.push_back("720p");
    PDef.push_back("480p");
    PDef.push_back("320p");


    for(int i=0;i<MaxP;++i){
        mParray[i] = new QAction(PDef.at(i), this);
        menuSelection->addAction(mParray[i]);
        connect(mParray[i],  &QAction::triggered, this, &SetVideoUrlDialog::slotBtnClick);
    }

//   QAction* mP1080            = new QAction(QStringLiteral("1080p"), this);
//   QAction* mP720             = new QAction(QStringLiteral("720p"), this);
//   QAction* mP480             = new QAction(QStringLiteral("480p"), this);
//   QAction* mP320             = new QAction(QStringLiteral("320p"), this);


//        menuSelection->addAction(mP1080);
//        menuSelection->addAction(mP720);
//        menuSelection->addAction(mP480);
//        menuSelection->addAction(mP320);

        //设置弹出菜单的样式QToolButton::MenuButtonPopup
        ui->toolButton->setPopupMode(QToolButton::MenuButtonPopup);
        //设置按钮样式
        ui->toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        //为按钮设置默认Action
        ui->toolButton->setDefaultAction(mParray[0]);
        //为按钮设置下拉菜单
        ui->toolButton->setMenu(menuSelection);


            connect(ui->pushButton_selectFile,   &QPushButton::clicked, this, &SetVideoUrlDialog::slotBtnClick);
//    connect(mP1080,  &QAction::triggered, this, &SetVideoUrlDialog::slotBtnClick);
//        connect(mP720,  &QAction::triggered, this, &SetVideoUrlDialog::slotBtnClick);
//            connect(mP480,  &QAction::triggered, this, &SetVideoUrlDialog::slotBtnClick);
//                connect(mP320,  &QAction::triggered, this, &SetVideoUrlDialog::slotBtnClick);


}

SetVideoUrlDialog::~SetVideoUrlDialog()
{
    delete ui;
}

void SetVideoUrlDialog::setVideoUrl(const QString &url)
{
    ui->lineEdit_fileUrl->setText(url);
}

QString SetVideoUrlDialog::getVideoUrl()
{
    QString url = ui->lineEdit_fileUrl->text();
    return url;
}

void SetVideoUrlDialog::setDownloadP(int defaultP){
    mDefaultP=defaultP;
}

int SetVideoUrlDialog::getDefaultP(){
    return mDefaultP;
}

void SetVideoUrlDialog::slotBtnClick(bool isChecked)
{
    if (QObject::sender() == ui->pushButton_selectFile)
    {
        QString s = QFileDialog::getOpenFileName(
                   this, QStringLiteral("选择要播放的文件"),
                    AppConfig::gVideoFilePath,//初始目录
                    QStringLiteral("视频文件 (*.flv *.rmvb *.avi *.mp4 *.mkv);;")
                    +QStringLiteral("音频文件 (*.mp3 *.wma *.wav *.m4a);;")
                    +QStringLiteral("所有文件 (*.*)"));
        if (!s.isEmpty())
        {
            ui->lineEdit_fileUrl->setText(s);
        }
    }

    for(int i=0;i<MaxP;i++){

        if (QObject::sender() == mParray[i])
        {
            ui->toolButton->setDefaultAction(mParray[i]);
            setDownloadP(i);
            //emit sig_defaultP(i);
            qDebug()<<"mParray:"<<mParray[i]<<endl;
            break;
        }
    }
}
