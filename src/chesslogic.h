#ifndef CHESSLOGIC_H
#define CHESSLOGIC_H

#include "chessboard.h"
#include <QObject>

class ChessLogic : public QObject
{
    Q_OBJECT
public:
    explicit ChessLogic(QObject *parent = nullptr);

public slots:
    void nextTurn();
    void gameOver(int flag);
    void showTurnResetButtonClicked();

signals:
    void TurnChanged(int new_turn);
    void ShowResult(int winner, int way);
    void ResetTurnSpinBox(int turn);

private:
    int m_turn;

};

#endif // CHESSLOGIC_H
