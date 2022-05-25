#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QObject>
#include <QVector>

class BoardData
{
public:
    BoardData();
    char getData(int col, int row);
    void setData(char ch, int col, int row);
    void reset();
private:
    char m_data[8][8];
};

class ChessBoard : public QObject
{
    Q_OBJECT
public:
    explicit ChessBoard(QObject *parent = nullptr);
    BoardData *getBoardData(int turn);
    BoardData *getHighLightMap();
    void resetHighLightMap();
    void updateHighLightMap(QPoint eg, int turn);
    bool moveChess(QPoint startat, QPoint moveto);
    bool getGameOverCheckingFlag();

signals:
    void chessMoved();
    void gameOver(int flag);

protected:
    int pieceplayer(char piece);
    void highLight_P(int turn, int col, int row);
    void highLight_p(int turn, int col, int row);
    void highLight_K(int turn, int col, int row);
    void highLight_k(int turn, int col, int row);
    void highLight_R(int turn, int col, int row);
    void highLight_r(int turn, int col, int row);
    void highLight_B(int turn, int col, int row);
    void highLight_b(int turn, int col, int row);
    void highLight_Q(int turn, int col, int row);
    void highLight_q(int turn, int col, int row);
    void highLight_N(int turn, int col, int row);
    void highLight_n(int turn, int col, int row);
    bool gridControlledByPlayer(int player, BoardData chessBoard, int col, int row);
    void checkGameOver();
    int checkKingExist();
    bool checkNoLegalMove();
    int checkCheck(int player, BoardData chessMap);
    void verifyHighLightMap(int player, BoardData chessMap, QPoint eg);


private:
    QVector<BoardData> m_boardhistory;
    BoardData m_highLightMap;
    BoardData m_chessMap;
    // 王车易位标志，false代表不能进行某种易位，分别为白王长、短易位和黑王长、短易位
    QVector<bool> castleFlag;
    QVector<QVector<bool>> flagHistory;
    // 游戏结束检查中标志，该标志为true时chesswidget不应接收新的输入，以免产生意外后果
    bool gameOverChecking;
};

#endif // CHESSBOARD_H
