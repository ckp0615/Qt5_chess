#ifndef CHESSVIEW_H
#define CHESSVIEW_H

#include "chessboard.h"
#include <QWidget>
#include <QMap>
#include <QIcon>
#include <QMouseEvent>

class ChessView : public QWidget
{
    Q_OBJECT
public:
    explicit ChessView(QWidget *parent = nullptr);
    void setBoard(ChessBoard *chessboard);

public slots:
    void nextTurn(int new_turn);
    void changeShowTurn(int show_turn);
    void setGameOverView();

protected:
    void drawGrid(QPainter *painter);
    void drawPieces(QPainter *painter, BoardData *data);
    void drawHighLight(QPainter *painter, BoardData *highlightmap);
    void drawDrag(QPainter *painter, BoardData *data);
    QRect gridRect(int column, int row);
    QPoint eventGrid(QPoint eventpoint);
    int pieceplayer(char piece);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

private:
    int gs = 80;
    QMap<char, QIcon> m_piece;
    ChessBoard *m_board;
    bool chessChosed;
    bool dragOn;
    QPoint mousePoint;
    QPoint m_chosedPiece;
    int m_drawturn;
    int m_maxturn;
    bool gameIsOver;
    bool isShowHistory;

};

#endif // CHESSVIEW_H
