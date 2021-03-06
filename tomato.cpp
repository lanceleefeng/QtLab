//
// Created by Administrator on 2017-5-16.
//

#include <QtWidgets>

#include <QDesktopWidget>
#include <QApplication>

#include <QPushButton>
#include <QCheckBox>

#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QIcon>

#include <QDebug>


#include "db.h"
#include "init.h"
#include "lineedit.h"

#include "tomato.h"
#include "unitlabel.h"

#include "settingmodel.h"

QString Tomato::name = "番茄倒计时";

Tomato::Tomato(QWidget *parent)
        : QWidget(parent)
        , countState(CountInit)
{


    /*Init init;
    if(!init.succeed()){
        //QMessageBox::critical(0, Tomato::name, QString("初始化失败"));
        //QMessageBox::critical(0, Tomato.name, QString("初始化失败"));
        //QMessageBox::critical(0, Tomato::name, QString("初始化失败"));
        //QMessageBox::critical(0, init.name, QString("初始化失败"));
        QMessageBox::critical(0, name, QString("初始化失败"));
        //return -1;
        //this->quit();

        return;
    }*/


    QDesktopWidget *desktop = QApplication::desktop();
    qDebug() << desktop->width() << "*"<< desktop->height();

    QRect availableRect = desktop->availableGeometry();

    qDebug() << "available: " << availableRect.width() << "*" << availableRect.height();

    int geoX = (availableRect.width() - width)/2;
    //int geoY = (availableRect.height() - height)/2;



    //setGeometry: Unable to set geometry 22x22+400+190 on QWidgetWindow/'TomatoClassWindow'.
    //Resulting geometry:  128x22+400+190 (frame: 8, 31, 8, 8, custom margin: 0, 0, 0, 0, minimum size: 22x22, maximum size: 16777215x16777215).

    setGeometry(geoX, 150, 500, 400);
    setWindowTitle(tr("番茄倒计时"));



    QLabel *progressTime = new QLabel("进度");
    progress = new QLabel("-");
    progressTime->setMinimumWidth(50);

    QLabel *toTime = new QLabel("到时时间");
    to = new QLabel("-");

    progressTime->setFixedWidth(labelMinWidth);
    progressTime->setFixedHeight(itemHeight);

    //progress->setFixedWidth(labelMinWidth);
    progress->setFixedHeight(itemHeight);

    toTime->setFixedWidth(labelMinWidth);
    toTime->setFixedHeight(itemHeight);

    //to->setFixedWidth(labelMinWidth);
    to->setFixedHeight(itemHeight);


    QHBoxLayout *progressInfoLayout = new QHBoxLayout;
    QHBoxLayout *toLayout = new QHBoxLayout;

    progressInfoLayout->addWidget(progressTime);
    progressInfoLayout->addWidget(progress);

    toLayout->addWidget(toTime);
    toLayout->addWidget(to);

    QLabel *time = new QLabel("时间");
    //time = new QLabel("时间");
    QLabel *tipTime = new QLabel("提示时间");
    QLabel *breakTime = new QLabel("休息时间");

    //time->setMinimumWidth(labelMinWidth);
    time->setFixedWidth(labelMinWidth);
    //time->setLineWidth()
    tipTime->setFixedWidth(labelMinWidth);
    breakTime->setFixedWidth(labelMinWidth);

    time->setFixedHeight(itemHeight);
    tipTime->setFixedHeight(itemHeight);
    breakTime->setFixedHeight(itemHeight);


    // 单行输入：QLineEdit
    // 多行输入：QTextEdit
    //QLineEdit *inputTime = new QLineEdit();
    //LineEdit *inputTime = new LineEdit;
    inputTime = new LineEdit;
    inputTipTime = new LineEdit();
    inputBreakTime = new LineEdit();

    inputTime->setText("25");
    inputTipTime->setText("3");
    inputBreakTime->setText("5");

    inputTime->setFixedWidth(inputWidth);
    inputTipTime->setFixedWidth(inputWidth);
    inputBreakTime->setFixedWidth(inputWidth);

    //inputTime->setFixedHeight(itemHeight);
    //inputTipTime->setFixedHeight(itemHeight);
    //inputBreakTime->setFixedHeight(itemHeight);



    //QLabel *unit = new QLabel("分钟");
    //unit->setFixedWidth(unitWidth);

    UnitLabel *timeUnit = new UnitLabel("分钟");
    UnitLabel *tipUnit = new UnitLabel("分钟");
    UnitLabel *breakUnit = new UnitLabel("分钟");

    timeUnit->setFixedHeight(itemHeight);
    tipUnit->setFixedHeight(itemHeight);
    breakUnit->setFixedHeight(itemHeight);


    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->addWidget(time);
    timeLayout->addWidget(inputTime);
    //timeLayout->addWidget(unit);
    timeLayout->addWidget(timeUnit);


    QHBoxLayout *tipLayout = new QHBoxLayout;
    tipLayout->addWidget(tipTime);
    tipLayout->addWidget(inputTipTime);
    tipLayout->addWidget(tipUnit);



    QHBoxLayout *breakLayout = new QHBoxLayout;
    breakLayout->addWidget(breakTime);
    breakLayout->addWidget(inputBreakTime);
    breakLayout->addWidget(breakUnit);


    timeLayout->setAlignment(Qt::AlignLeft);
    tipLayout->setAlignment(Qt::AlignLeft);
    breakLayout->setAlignment(Qt::AlignLeft);


    startButton = new QPushButton("开始", this);
    restartButton = new QPushButton("重新计时", this);

    continueButton = new QPushButton("继续", this);
    pauseButton = new QPushButton("暂停", this);
    stopButton = new QPushButton("停止", this);


    breakButton = new QPushButton("休息", this);

    addButton = new QPushButton("添加元素", this);
    addButton->setFixedWidth(buttonWidth + 20);
    addButton->hide();
    connect(addButton, &QPushButton::clicked, this, &addButtonClicked);

    startButton->setFixedWidth(buttonWidth);

    restartButton->setFixedWidth(buttonWidth + 20);
    continueButton->setFixedWidth(buttonWidth);
    pauseButton->setFixedWidth(buttonWidth);
    stopButton->setFixedWidth(buttonWidth);
    breakButton->setFixedWidth(buttonWidth);


    //startButton->hide();
    //restartButton->hide();
    //pauseButton->hide();
    //stopButton->hide();
    //breakButton->hide();

    setControlButtonVisibility();

    // 右侧设置

    //QLabel *settingsTitle = new QLabel("设置");
    QLabel *settingsTitle = new QLabel("设置", this);
    settingsTitle->setFixedHeight(itemHeight);

    //正数计数不易理解，文字改成“倒计时”
    //勾选就是倒计时，递减，不勾选是正计时，递增
    //QCheckBox *increaseCheckBox = new QCheckBox("正数计数", this);
    //increaseCheckBox->setChecked(increase);
    QCheckBox *countdownCheckBox = new QCheckBox("倒计时", this);

    countdownCheckBox->setToolTip("勾选则进行倒计时，不勾选时正数计时");
    countdownCheckBox->setChecked(!increase);

    connect(countdownCheckBox, &QCheckBox::stateChanged, this, &Tomato::countModeChanged);

    QCheckBox *autoStartCheckBox = new QCheckBox("自动开始", this);
    autoStartCheckBox->setToolTip("启动时是否自动开始计时（使用最近设置）");
    autoStartCheckBox->setChecked(autoStart);




    QVBoxLayout *progressLayout = new QVBoxLayout;
    QVBoxLayout *formLayout = new QVBoxLayout;
    QHBoxLayout *controlLayout = new QHBoxLayout;

    // 无效
    //progressLayout->setGeometry(QRect(0, 0, 150, 150));
    //formLayout->setGeometry(QRect(0, 160, 150, 50));
    //controlLayout->setGeometry(QRect(0, 320, 150, 50));


    //progressLayout->setAlignment(Qt::AlignTop);
    progressLayout->addLayout(progressInfoLayout);
    progressLayout->addLayout(toLayout);

    //tipLayout->setSizeConstraint()
    //tipLayout->setMax

    //timeLayout->setGeometry(QRect(0, 0, 100, 100));
    //tipLayout->setGeometry(QRect(0, 0, 100, 100));
    //breakLayout->setGeometry(QRect(0, 0, 100, 100));


    formLayout->setContentsMargins(0, 24, 0, 0);
    formLayout->addLayout(timeLayout);
    formLayout->addLayout(tipLayout);
    formLayout->addLayout(breakLayout);

    //formLayout->setMargin(50);
    //formLayout->setContentsMargins(50, 5, 0, 0);

    //controlLayout->setAlignment(Qt::AlignTop);

    controlLayout->addWidget(startButton);
    controlLayout->addWidget(restartButton);
    controlLayout->addWidget(continueButton);
    controlLayout->addWidget(pauseButton);
    controlLayout->addWidget(stopButton);
    controlLayout->addWidget(breakButton);

    controlLayout->addWidget(addButton);

    controlLayout->setAlignment(Qt::AlignLeft);



    QBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setAlignment(Qt::AlignTop);
    //leftLayout->setContentsMargins(50, 10, 50, 10);

    leftLayout->addLayout(progressLayout);
    //leftLayout->addStretch(1);
    leftLayout->addLayout(formLayout);
    //leftLayout->addLayout(controlLayout);

    //QBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout = new QVBoxLayout;
    rightLayout->setAlignment(Qt::AlignTop);
    //rightLayout->setMargin(0);
    rightLayout->addWidget(settingsTitle);


    rightLayout->addWidget(autoStartCheckBox);
    //rightLayout->addStretch(1);
    rightLayout->addWidget(countdownCheckBox);
    //rightLayout->addStretch(1);

    QBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    QBoxLayout *layout = new QVBoxLayout;
    //layout->setContentsMargins(50, 30, 50, 10);
    layout->setContentsMargins(50, 50, 50, 10);

    layout->addLayout(mainLayout);
    layout->addStretch(1);
    layout->addLayout(controlLayout);
    //layout->addStretch(1);
    layout->addStretch(3);


    //setLayout(mainLayout);
    setLayout(layout);




    // ------------------ 布局、排版结束 --------------------

    // ------------------ 交互操作 --------------------------

    connect(startButton, &QPushButton::clicked, this, &startCountDown);
    connect(restartButton, &QPushButton::clicked, this, &restart);
    connect(continueButton, &QPushButton::clicked, this, &continueCountDown);
    connect(pauseButton, &QPushButton::clicked, this, &pause);
    connect(stopButton, &QPushButton::clicked, this, &stop);
    connect(breakButton, &QPushButton::clicked, this, &startBreak);

    taskbarButton = new QWinTaskbarButton(this);
    taskbarProgress = taskbarButton->progress();
    taskbarProgress->setVisible(true);


    initiated = true;

}

Tomato::~Tomato()
{

}

void Tomato::startCountDown()
{

    setCountState(CountInProgress);
    //countState = CountInProgress;
    //setControlButtonVisibility();

    taskbarButton->setWindow(this->windowHandle());

    //qDebug() << "开始计时...";
    //qDebug() << "time: " << inputTime->text() << ", tip time: " << inputTipTime->text() << ", break time: " << inputBreakTime->text();

    row["time"] = inputTime->text();
    row["tipTime"] = inputTipTime->text();
    row["breakTime"] = inputBreakTime->text();

    row["from_time"] = QDateTime::currentDateTime();
    row["to_time"] = row["from_time"].toDateTime().addSecs(row["time"].toInt()*60);

    to->setText(row["to_time"].toDateTime().toString("yyyy-MM-dd hh:mm::ss"));

    //row["timer"] = new QTimer();
    timer[key] = new QTimer();

    //connect(row["timer"], &QTimer::timeout, this, &Tomato::countDown);
    //connect(row["timer"].toTimier(), &QTimer::timeout, this, &Tomato::countDown);
    connect(timer[key], &QTimer::timeout, this, &Tomato::countDown);
    timer[key]->start(1000);
    countDown();
}


void Tomato::restart()
{
    //countState = CountInProgress;
    setCountState(CountInProgress);
    timeConsumed[key] = 0;


    row["time"] = inputTime->text();
    row["tipTime"] = inputTipTime->text();
    row["breakTime"] = inputBreakTime->text();

    row["from_time"] = QDateTime::currentDateTime();
    row["to_time"] = row["from_time"].toDateTime().addSecs(row["time"].toInt()*60);
    to->setText(row["to_time"].toDateTime().toString("yyyy-MM-dd hh:mm::ss"));

    //row["timer"] = new QTimer();
    //timer[key] = new QTimer();
    timer[key]->stop();

    //connect(timer[key], &QTimer::timeout, this, &Tomato::countDown);
    timer[key]->start(1000);
    countDown();
}


void Tomato::continueCountDown()
{
    setCountState(CountInProgress);
    row["from_time"] = QDateTime::currentDateTime();

    // 需要重新计算到时时间
    row["to_time"] = row["from_time"].toDateTime().addSecs(row["time"].toInt()*60 - timeConsumed[key]);
    to->setText(row["to_time"].toDateTime().toString("yyyy-MM-dd hh:mm::ss"));

    countDown();
}

void Tomato::pause()
{
    //timeConsumed[key] = getSecondsSince(row["from_time"].toDateTime());
    timeConsumed[key] += getSecondsSince(row["from_time"].toDateTime());
    qDebug() << "已耗时：" << timeConsumed[key] << "s";

    setCountState(CountPause);
    row["from_time"] = QDateTime::currentDateTime();
    countDown();
}

void Tomato::stop()
{
    if(countState == CountInProgress){
        timeConsumed[key] += getSecondsSince(row["from_time"].toDateTime());
    }
    setCountState(CountStop);
    row["from_time"] = QDateTime::currentDateTime();
    to->setText("-");
    //timer[key]->stop();
    countDown();
}

void Tomato::startBreak()
{

    //timeConsumed[key] += getSecondsSince(row["from_time"].toDateTime());
    if(countState == CountInProgress){
        timeConsumed[key] += getSecondsSince(row["from_time"].toDateTime());
    }
    setCountState(CountBreak);

    row["breakTime"] = inputBreakTime->text();
    row["from_time"] = QDateTime::currentDateTime();

    row["to_time_break"] = row["from_time"].toDateTime().addSecs(row["breakTime"].toInt()*60);
    to->setText(row["to_time_break"].toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    countDown();
}


void Tomato::countDown()
{

    qint64 secs = getSecondsSince(row["from_time"].toDateTime());
    row["secs"] = secs;

    qint64 m = secs/60;
    qint64 s = secs%60;

    qint64 consumedMin = timeConsumed[key]/60;
    qint64 consumedSec = timeConsumed[key]%60;

    qint64 time = row["time"].toInt();
    qint64 tipTime = row["tipTime"].toInt();
    qint64 breakTime = row["breakTime"].toInt();

    QString progressInfo;
    //QString state;
    qint64 overlayMin, titleMin, titleSec;
    qint64 value = 0;
    qint64 maxValue = 0;


    titleSec = s;
    overlayMin = m;

    value = secs;
    maxValue = 0;

    if(increase){
        switch(countState)
        {
            case CountInProgress:

                secs += timeConsumed[key];
                if(secs >= time*60){
                //if(secs + timeConsumed[key] >= time*60){
                    startBreak();
                    return;
                }

                //timeConsumed[key] = secs;
                state = (secs < (time - tipTime)*60) ? Normal : Stop;

                overlayMin = secs/60;
                titleSec = secs%60;

                value = secs;
                maxValue = time*60;
                progressInfo = QString("%1:%2/%3").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(time);

                break;
            case CountPause:
                //state = Pause;
                state = Normal;
                progressInfo = QString("暂停 %1:%2 - %3:%4/%5").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(paddingZero(consumedMin)).arg(paddingZero(consumedSec)).arg(time);
                break;
            case CountStop:
                //state = Stop;
                state = Normal;
                progressInfo = QString("停止 %1:%2 - %3:%4/%5").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(paddingZero(consumedMin)).arg(paddingZero(consumedSec)).arg(time);
                break;
            case CountBreak:
                if(secs >= breakTime*60){

                    //timeConsumed[key] += getSecondsSince(row["from_time"].toDateTime());
                    //if(countState == CountInProgress){
                    //    timeConsumed[key] += getSecondsSince(row["from_time"].toDateTime());
                    //}
                    setCountState(CountOvertime);
                    row["from_time"] = QDateTime::currentDateTime();
                    countDown();
                    return;
                }
                qDebug() << "开始休息：" << secs << "s";
                state = Pause;
                maxValue = breakTime*60;

                if(timeConsumed[key] >= time*60){
                    progressInfo = QString("休息 %1:%2/%3 - %4").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(breakTime).arg(time);
                }else{
                    progressInfo = QString("休息 %1:%2/%3 - %4:%5/%6").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(breakTime).arg(paddingZero(consumedMin)).arg(paddingZero(consumedSec)).arg(time);
                }

                break;
            case CountOvertime:
                state = Normal;
                //state = Stop;
                //progressInfo = QString("超时 %1:%2 - %3-%4").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(time).arg(breakTime);
                //if(consumedMin >= time){
                if(timeConsumed[key] >= time*60){
                    progressInfo = QString("超时 %1:%2 - %3-%4").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(time).arg(breakTime);
                }else{
                    progressInfo = QString("超时 %1:%2 - %3:%4/%5-%6").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(paddingZero(consumedMin)).arg(paddingZero(consumedSec)).arg(time).arg(breakTime);
                }
                break;
            default:

                break;
        }
    }else{

        //titleSec == (s == 0) ? 0 : (60 - s);

        // 倒数计时时显示剩余的分钟、秒数
        if(consumedSec == 0){
            consumedMin = time - consumedMin;
        }else{
            consumedMin = time - consumedMin - 1;
            consumedSec = 60 - consumedSec;
        }


        switch(countState)
        {
            case CountInProgress:

                secs += timeConsumed[key];
                if(secs >= time*60){
                    //if(secs + timeConsumed[key] >= time*60){
                    startBreak();
                    return;
                }

                //timeConsumed[key] = secs;
                state = (secs < (time - tipTime)*60) ? Normal : Stop;

                overlayMin = secs/60;
                titleSec = secs%60;

                // 倒数时处理进位
                titleMin = (titleSec == 0) ? (time - overlayMin) : (time - overlayMin - 1);
                titleSec = (titleSec == 0) ? 0 : (60 - titleSec);
                overlayMin = time - m;

                //value = secs;
                maxValue = time*60;
                value = maxValue - secs;

                progressInfo = QString("%1:%2/%3").arg(paddingZero(titleMin)).arg(paddingZero(titleSec)).arg(time);

                break;
            case CountPause:
                //state = Pause;
                state = Normal;
                progressInfo = QString("暂停 %1:%2 - %3:%4/%5").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(paddingZero(consumedMin)).arg(paddingZero(consumedSec)).arg(time);
                break;
            case CountStop:
                //state = Stop;
                state = Normal;
                progressInfo = QString("停止 %1:%2 - %3:%4/%5").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(paddingZero(consumedMin)).arg(paddingZero(consumedSec)).arg(time);
                break;
            case CountBreak:
                if(secs >= breakTime*60){
                    setCountState(CountOvertime);
                    row["from_time"] = QDateTime::currentDateTime();
                    countDown();
                    return;
                }
                qDebug() << "开始休息：" << secs << "s";
                state = Pause;
                maxValue = breakTime*60;
                value = maxValue - secs;

                //倒数时处理进位
                //倒数时什么样上的分钟数比界面显示的分钟数大1，
                //原因是，如果不大1，会出现任务栏上分钟数为0，却仍然没有结束，还有1分钟才结束

                titleMin = (titleSec == 0) ? (breakTime - overlayMin) : (breakTime - overlayMin - 1);
                titleSec = (titleSec == 0) ? 0 : (60 - titleSec);
                overlayMin = breakTime - m;

                if(timeConsumed[key] >= time*60){
                    progressInfo = QString("休息 %1:%2/%3 - %4").arg(paddingZero(titleMin)).arg(paddingZero(titleSec)).arg(breakTime).arg(time);
                }else{
                    progressInfo = QString("休息 %1:%2/%3 - %4:%5/%6").arg(paddingZero(titleMin)).arg(paddingZero(titleSec)).arg(breakTime).arg(paddingZero(consumedMin)).arg(paddingZero(consumedSec)).arg(time);
                }

                break;
            case CountOvertime:
                state = Normal;
                //state = Stop;
                //progressInfo = QString("超时 %1:%2 - %3-%4").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(time).arg(breakTime);
                // 倒计时时consumedMin表示剩余分钟，不能用于判断
                //if(consumedMin >= time){
                if(timeConsumed[key] >= time*60){
                    progressInfo = QString("超时 %1:%2 - %3-%4").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(time).arg(breakTime);
                }else{
                    progressInfo = QString("超时 %1:%2 - %3:%4/%5-%6").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(paddingZero(consumedMin)).arg(paddingZero(consumedSec)).arg(time).arg(breakTime);
                }
                break;
            default:

                break;
        }

    }


    //setTitle(progressInfo);
    //setProgress(progressInfo);

    setWindowTitle(QString("%1 - %2").arg(progressInfo).arg(name));
    progress->setText(progressInfo);

    setOverlayIcon(overlayMin);
    setTaskbarProgress(state, value, maxValue);

    return;

    // 最初只有开始，没有其它控制按钮的代码：


    if(increase){
        titleSec = s;
        if(secs < time*60){
            state = (secs < (time - tipTime)*60) ? Normal : Stop;
            overlayMin = m;
            value = secs;
            maxValue = time*60;
            //progressInfo = QString("%1:%2/%3").arg(overlayMin).arg(titleSec).arg(time);
            //progressInfo = QString("%1:%2/%3").arg(overlayMin, 2, 10, QLatin1Char('0')).arg(titleSec).arg(time);
            progressInfo = QString("%1:%2/%3").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(time);

        }else if(secs < (time + breakTime)*60){
            //state = "pause";
            state = Pause;
            overlayMin = m - time;
            value = secs - time*60;
            maxValue = breakTime*60;
            progressInfo = QString("休息 %1:%2/%3 - %4").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(breakTime).arg(time);

        }else{
            state = Normal;
            overlayMin = m - time - breakTime;
            progressInfo = QString("超时 %1:%2 - %3-%4").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(time).arg(breakTime);
        }
    }else{

        //titleSec = 60 - s;
        titleSec = (s == 0) ? 0 : (60 -s);

        if(secs < time*60){
            state = (secs < (time - tipTime)*60) ? Normal : Stop;
            overlayMin = time - m;
            titleMin = (s == 0) ? (time-m) : (time - m - 1);
            //value = secs;
            value = time*60 - secs;
            maxValue = time*60;
            //progressInfo = QString("%1:%2/%3").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(time);
            progressInfo = QString("%1:%2/%3").arg(paddingZero(titleMin)).arg(paddingZero(titleSec)).arg(time);

        }else if(secs < (time + breakTime)*60){
            //state = "pause";
            state = Pause;
            overlayMin = time + breakTime - m;
            titleMin = (s == 0) ? time + breakTime - m : time + breakTime - m - 1;

            value = (time + breakTime)*60 - secs;
            maxValue = breakTime*60;
            progressInfo = QString("休息 %1:%2/%3 - %4").arg(paddingZero(titleMin)).arg(paddingZero(titleSec)).arg(breakTime).arg(time);
        }else{
            state = Normal;
            titleSec = s;
            overlayMin = m - time - breakTime;
            progressInfo = QString("超时 %1:%2 - %3-%4").arg(paddingZero(overlayMin)).arg(paddingZero(titleSec)).arg(time).arg(breakTime);
        }

    }


    //setTitle(progressInfo);
    //setProgress(progressInfo);

    setWindowTitle(QString("%1 - %2").arg(progressInfo).arg(name));
    progress->setText(progressInfo);

    setOverlayIcon(overlayMin);
    setTaskbarProgress(state, value, maxValue);


}


// 使用槽函数接收increaseCheckBox，不必把increaseCheckBox当作Tomato的成员了
//void Tomato::increaseChanged(Qt::CheckState state)
void Tomato::countModeChanged(int state)
{
    QString key = "saveCountMode";
    qDebug() << "increase: " << state;
    //increase = state == Qt::Checked ? true : false;
    //increase = state == Qt::Checked;
    //oldSettings[key] = increase;
    increase = !(state == Qt::Checked);
    countDown();
    //settings["countMode"] =
    newSettings["saveCountMode"] = increase;
    beginSaveCountMode();
}

void Tomato::setOverlayIcon(int min)
{

    // 优化空间：
    // 每秒执行一次，但min是60秒，如果min没有变，没有必要重新绘制!

    if(min == lastOverlayMin){
        return;
    }
    lastOverlayMin = min;

    //icon = new QPixmap(16, 16);
    //icon = QPixmap(16, 16);
    QPixmap icon(16, 16);
    icon.fill(Qt::transparent);
    QPainter painter(&icon);

    QFont font("Times", 10, QFont::ExtraBold);
    painter.setFont(font);

    painter.setPen(Qt::white);

    //重叠是因为pixmap重复绘制导致的，而不是没有清空icon
    //taskbarButton->clearOverlayIcon();

    //QPixmap::fill: Cannot fill while pixmap is being painted on
    //QPaintDevice: Cannot destroy paint device that is being painted

    // 关闭窗口时报错：
    //QPaintDevice: Cannot destroy paint device that is being painted

    QString text = QString("%1").arg(min);
    //icon->fill(Qt::transparent);
    //icon.fill(Qt::transparent);
    QRect rect(0, 0, 16, 16);
    painter.drawText(rect, Qt::AlignCenter, text);
    taskbarButton->setOverlayIcon(QIcon(icon));
}

void Tomato::setTaskbarProgress(State state, int value, int maxValue)
//void Tomato::setTaskbarProgress(int state, int value, int maxValue)
{

    //qDebug() << value << "/" << maxValue;
    //qDebug() << "state: " << state;

    value = value > progressMinValue ? value : progressMinValue;
    switch(state){
        case Pause:
            // 停止的要暂停需要先恢复
            taskbarProgress->resume();
            taskbarProgress->pause();
            break;
        case Stop:
            taskbarProgress->stop();
            break;
        default:
            taskbarProgress->resume();
            break;
    }

    if(maxValue > 0){
        taskbarProgress->setMaximum(maxValue);
        taskbarProgress->setValue(value);
    }else{
        taskbarProgress->setRange(0, 0);
    }

}

void Tomato::setCountState(CountState state)
{
    countState = state;
    setControlButtonVisibility();
}


QString Tomato::paddingZero(qint64 i)
{
    QString s = QString("%1").arg(i, 2, 10, QLatin1Char('0'));
    return s;
}

qint64 Tomato::getSecondsSince(QDateTime time)
{
    qint64 mSecs = QDateTime::currentMSecsSinceEpoch() - row["from_time"].toDateTime().toMSecsSinceEpoch();
    return mSecs/1000;
}

void Tomato::setControlButtonVisibility()
{
    switch(countState)
    {
        case CountInProgress:
            startButton->hide();
            restartButton->show();
            continueButton->hide();
            pauseButton->show();
            stopButton->show();
            breakButton->show();
            break;
        case CountPause:
            startButton->hide();
            restartButton->show();
            continueButton->show();
            pauseButton->hide();
            stopButton->show();
            breakButton->show();
            break;
        case CountStop:
            startButton->hide();
            restartButton->show();
            continueButton->hide();
            pauseButton->hide();
            stopButton->hide();
            breakButton->show();
            break;
        case CountBreak:
        case CountOvertime:
            startButton->hide();
            restartButton->show();
            continueButton->hide();
            pauseButton->hide();
            stopButton->hide();
            breakButton->show();
            break;
        case CountInit:
            //break;
        default:
            startButton->show();
            restartButton->hide();
            continueButton->hide();
            pauseButton->hide();
            stopButton->hide();
            breakButton->show();
            break;
    }
}


void Tomato::addButtonClicked()
{
    //qDebug() << "动态增加按钮..."; // 可以
    //rightLayout->addWidget(new QCheckBox("test"));
}


void Tomato::beginSaveCountMode()
{

    QString key = "saveCountMode";
    //qDebug() << delayedActions[key];
    //timer[key] = new QTimer;
    if(delayedActions[key]){
        return;
    }
    delayedActions[key] = true;
    QTimer::singleShot(3000, this, SLOT(endSaveCountMode()));

}

void Tomato::endSaveCountMode()
{

    delayedActions["saveCountMode"] = false;

    QString key = "saveCountMode";

    qDebug() << "IN " << __FUNCTION__ << ", 执行保存程序";

    //if(newSettings[key] == increase)

    if(newSettings[key].toBool() == oldSettings[key].toBool()){
        qDebug() << "与原值相同，不保存";
        return;
    }
    oldSettings[key] = increase;

    int countDown = increase ? 0 : 1;

    SettingModel settingModel;
    settingModel.saveCountMode(countDown);

}



