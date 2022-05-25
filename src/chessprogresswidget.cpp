#include "chessprogresswidget.h"
#include "qdebug.h"
#include "ui_chessprogresswidget.h"
#include <QPainter>
#include <QInputDialog>
#include <QMessageBox>

ChessProgressWidget::ChessProgressWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChessProgressWidget)
{
    ui->setupUi(this);
    m_playerIcon.insert(0, QIcon(":/img/Chess_klt45.svg"));
    m_playerIcon.insert(1, QIcon(":/img/Chess_kdt45.svg"));
    // 初始化变量
    player = 0;
    ui->playerLabel->setText("白方");
    isPause = true;
    isTimeOut = false;
    ui->pauseButton->setIcon(QIcon(":/img/24gf-play.svg"));

    total_time = remain_time = 30000;
    timeInterval = 50;
    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
    m_timer->setInterval(timeInterval);
    // m_timer->start();

    connect(m_timer, &QTimer::timeout, this, &ChessProgressWidget::timeUpdate);
    connect(ui->pauseButton, &QPushButton::clicked, this, &ChessProgressWidget::pauseButtonClicked);
    connect(ui->settingButton, &QPushButton::clicked, this, &ChessProgressWidget::settingButtonClicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &ChessProgressWidget::resetButtonClicked);
    connect(ui->offerDrawButton, &QPushButton::clicked, this, &ChessProgressWidget::offerDrawButtonClicked);
    connect(ui->admitDefeatButton, &QPushButton::clicked, this, &ChessProgressWidget::admitDefeatButtonClicked);
}

ChessProgressWidget::~ChessProgressWidget()
{
    delete ui;
}

void ChessProgressWidget::drawPlayerIcon(QPainter *painter)
{
    m_playerIcon.value(player).paint(painter, QRect(width()/24, 0, height(), height()));
}

void ChessProgressWidget::drawTimeBar(QPainter *painter)
{
    int total_len = 0.75*width();
    // 绘制外框
    painter->setPen(Qt::black);
    painter->drawRect(width()/24+height()+width()/24, 0.7*height(), total_len+1, 0.2*height());
    // 按剩余时间比例绘制内条
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::green);
    int remain_len = ((double)remain_time / total_time) * total_len;
    painter->drawRect(1+width()/24+height()+width()/24, 1+0.7*height(), remain_len, 0.2*height()-1);
}

void ChessProgressWidget::showRemainTime()
{
    int min = remain_time / 60000;
    int sec = (remain_time % 60000) / 1000;
    if(sec < 10){
        ui->timeLable->setText(QString::number(min)+":0"+QString::number(sec));
    } else {
        ui->timeLable->setText(QString::number(min)+":"+QString::number(sec));
    }
}

void ChessProgressWidget::timeUpdate()
{
    remain_time -= timeInterval;
    if(remain_time <= 0){
        remain_time = 0;
        m_timer->stop();
        // 处理超时
        isTimeOut = true;
        QMessageBox timeOutMsg;
        timeOutMsg.setWindowTitle("行动超时");
        if(player == 0){
            timeOutMsg.setText("白方行动超时！");
        } else {
            timeOutMsg.setText("黑方行动超时！");
        }
        timeOutMsg.exec();
    }
    update();
}

void ChessProgressWidget::pauseButtonClicked()
{
    if(isPause){
        ui->pauseButton->setIcon(QIcon(":/img/24gf-pause2.svg"));
        if(!isTimeOut){
            m_timer->start();
        }
        isPause = false;
    } else {
        ui->pauseButton->setIcon(QIcon(":/img/24gf-play.svg"));
        m_timer->stop();
        isPause = true;
    }
}

void ChessProgressWidget::settingButtonClicked()
{
    bool ok;
    int time_sec = QInputDialog::getInt(this, tr("计时器设置"), tr("请输入每步玩家行动时间（秒）："), 30, 1, 5999, 1, &ok);
    if(ok){
        total_time = remain_time = time_sec * 1000;
    }
}

void ChessProgressWidget::resetButtonClicked()
{
    QMessageBox resetBox;
    resetBox.setWindowTitle("重新开局");
    resetBox.setText("要重新开始一局游戏吗？本局游戏的数据将会丢失");
    resetBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    resetBox.setDefaultButton(QMessageBox::Cancel);
    int ret = resetBox.exec();
    if(ret == QMessageBox::Yes){
        emit resetGame();
    }
}

void ChessProgressWidget::offerDrawButtonClicked()
{
    // 发起求和
    QMessageBox drawBox;
    drawBox.setWindowTitle("发起和棋");
    if(player == 0){
        drawBox.setText("白方发起了和棋申请，黑方要接受吗？");
    } else {
        drawBox.setText("黑方发起了和棋申请，白方要接受吗？");
    }
    drawBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    drawBox.setDefaultButton(QMessageBox::Cancel);
    // 等待对方确认时，暂停计时器
    m_timer->stop();
    int ret = drawBox.exec();
    if(ret == QMessageBox::Yes){
        emit gameOver(5);
    } else {
        m_timer->start();
    }
}

void ChessProgressWidget::admitDefeatButtonClicked()
{
    QMessageBox adBox;
    adBox.setWindowTitle("投子认负");
    if(player == 0){
        adBox.setText("白方确定要投子认负吗？");
    } else {
        adBox.setText("黑方确定要投子认负吗？");
    }
    adBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    adBox.setDefaultButton(QMessageBox::Cancel);
    int ret = adBox.exec();
    if(ret == QMessageBox::Yes){
        if(player == 0){
            emit gameOver(6);
        } else {
            emit gameOver(7);
        }
    }
}

void ChessProgressWidget::newTurn(int newturn)
{
    // 更新玩家
    player = newturn % 2;
    if(player == 0){
        ui->playerLabel->setText("白方");
    } else {
        ui->playerLabel->setText("黑方");
    }
    // 重置时间
    remain_time = total_time;
    isTimeOut = false;
    // 开始倒计时
    isPause = false;
    ui->pauseButton->setIcon(QIcon(":/img/24gf-pause2.svg"));
    m_timer->start();
}

void ChessProgressWidget::setGameOverView()
{
    m_timer->stop();
    ui->offerDrawButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->settingButton->setEnabled(false);
    ui->admitDefeatButton->setEnabled(false);
}

void ChessProgressWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();
    drawPlayerIcon(&painter);
    painter.restore();

    painter.save();
    drawTimeBar(&painter);
    painter.restore();

    showRemainTime();
}

void ChessProgressWidget::resizeEvent(QResizeEvent *event)
{
    // 走子方和剩余时间的label位置设置
    ui->playerLabel->setGeometry(width()/24+height()+width()/24, 10, 45, 21);
    int p = 50 < width()*0.13 ? 50 : width()*0.15;
    ui->timeLable->setGeometry(width()/12+height()+p, 10, 80, 21);
    // QPushButton的位置设置
    ui->pauseButton->setGeometry(width()*0.45, 0, 40, 40);
    ui->settingButton->setGeometry(width()*0.45+45, 0, 40, 40);
    ui->resetButton->setGeometry(width()*0.45+115, 0, 40, 40);
    ui->offerDrawButton->setGeometry(width()*0.45+160, 0, 40, 40);
    ui->admitDefeatButton->setGeometry(width()*0.45+205, 0, 40, 40);
}
