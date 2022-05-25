#ifndef CHESSPROGRESSWIDGET_H
#define CHESSPROGRESSWIDGET_H

#include "qicon.h"
#include <QWidget>
#include <QMap>
#include <QPainter>
#include <QTimer>

namespace Ui {
class ChessProgressWidget;
}

class ChessProgressWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChessProgressWidget(QWidget *parent = nullptr);
    ~ChessProgressWidget();

protected:
    void drawPlayerIcon(QPainter *painter);
    void drawTimeBar(QPainter *painter);
    void showRemainTime();
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:
    void resetGame();
    void gameOver(int flag);

public slots:
    void newTurn(int newturn);
    void setGameOverView();

protected slots:
    void timeUpdate();
    void pauseButtonClicked();
    void settingButtonClicked();
    void resetButtonClicked();
    void offerDrawButtonClicked();
    void admitDefeatButtonClicked();

private:
    Ui::ChessProgressWidget *ui;
    QMap<int, QIcon> m_playerIcon;
    bool isPause;
    bool isTimeOut;
    int timeInterval;
    long remain_time, total_time;
    QTimer *m_timer;
    int player;

};

#endif // CHESSPROGRESSWIDGET_H
