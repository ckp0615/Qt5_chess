#include "chesslogic.h"
#include "qpoint.h"

ChessLogic::ChessLogic(QObject *parent)
    : QObject{parent}
{
    m_turn = 0;
}

void ChessLogic::nextTurn()
{
    m_turn += 1;
    emit TurnChanged(m_turn);
}

void ChessLogic::gameOver(int flag)
{
    if(flag == -2 || flag == -1 || flag == 0){
        // 王被杀，除调试以外不应触发
        if(flag == 0){
            emit ShowResult(0, -1);
        } else if(flag == -1){
            emit ShowResult(1, -1);
        } else{
            emit ShowResult(2, -1);
        }
    } else if(flag == 1 || flag == 2){
        // checkmate!
        if(flag == 1){
            emit ShowResult(0, 0);
        } else {
            emit ShowResult(1, 0);
        }
    } else if(flag == 3 || flag == 4){
        // stalemate
        if(flag == 3){
            emit ShowResult(2, 1);
        } else {
            emit ShowResult(3, 1);
        }
    } else if(flag == 5){
        // 双方协商后平局
        emit ShowResult(2, 2);
    } else if(flag == 6 || flag == 7){
        // 投子认负
        if(flag == 6){
            // 白方认负
            emit ShowResult(1, 3);
        } else {
            emit ShowResult(0, 3);
        }
    }
}

void ChessLogic::showTurnResetButtonClicked()
{
    emit ResetTurnSpinBox(m_turn);
}
