#include "chessboard.h"
#include "promotiondialog.h"
#include "qdebug.h"
#include "qmessagebox.h"
#include "qpoint.h"

ChessBoard::    ChessBoard(QObject *parent)
    : QObject{parent}
{
    BoardData d = BoardData();

    std::string p1 = "RNBQKBNR";
    std::string p2 = "PPPPPPPP";
    std::string p7 = "pppppppp";
    std::string p8 = "rnbqkbnr";
    for(int c=1; c < 9; c++){
        d.setData(p1[c-1], c, 1);
        d.setData(p2[c-1], c, 2);
        d.setData(p7[c-1], c, 7);
        d.setData(p8[c-1], c, 8);
    }
    m_boardhistory.append(d);
    m_chessMap = BoardData(d);
    for(int i = 0; i < 4; i++){
        castleFlag.append(true);
    }
    flagHistory.append(castleFlag);
    gameOverChecking = false;
}

BoardData *ChessBoard::getBoardData(int turn)
{
    if(turn > m_boardhistory.size()){
        return NULL;
    } else if(turn == m_boardhistory.size()){
        return &m_chessMap;
    } else {
        return &m_boardhistory[turn];
    }
}

BoardData *ChessBoard::getHighLightMap()
{
    return &m_highLightMap;
}

void ChessBoard::resetHighLightMap()
{
    m_highLightMap.reset();
}

void ChessBoard::updateHighLightMap(QPoint eg, int turn)
{
    m_highLightMap.reset();
    BoardData chessMap = m_boardhistory.at(turn);
    char piece = chessMap.getData(eg.x(),eg.y());
    if(pieceplayer(piece) == 0){
        m_highLightMap.setData('A', eg.x(), eg.y());
    } else if (pieceplayer(piece) == 1){
        m_highLightMap.setData('a', eg.x(), eg.y());
    }
    switch(piece){
    case 'P':
        highLight_P(turn, eg.x(), eg.y());
        break;
    case 'p':
        highLight_p(turn, eg.x(), eg.y());
        break;
    case 'K':
        highLight_K(turn, eg.x(), eg.y());
        break;
    case 'k':
        highLight_k(turn, eg.x(), eg.y());
        break;
    case 'R':
        highLight_R(turn, eg.x(), eg.y());
        break;
    case 'r':
        highLight_r(turn, eg.x(), eg.y());
        break;
    case 'B':
        highLight_B(turn, eg.x(), eg.y());
        break;
    case 'b':
        highLight_b(turn, eg.x(), eg.y());
        break;
    case 'Q':
        highLight_Q(turn, eg.x(), eg.y());
        break;
    case 'q':
        highLight_q(turn, eg.x(), eg.y());
        break;
    case 'N':
        highLight_N(turn, eg.x(), eg.y());
        break;
    case 'n':
        highLight_n(turn, eg.x(), eg.y());
        break;
    }
}

bool ChessBoard::moveChess(QPoint st, QPoint eg)
{
    if(m_highLightMap.getData(st.x(),st.y()) != 'A' && m_highLightMap.getData(st.x(),st.y()) != 'a'){
        return false;
    }
    char hl = m_highLightMap.getData(eg.x(), eg.y());
    if(hl == 'm' || hl == 'M' || hl == 'e' || hl == 'E' || hl == 'r' || hl == 'R'){
        // 兵升变
        if(hl == 'r' || hl == 'R'){
            PromotionDialog dialogbox(pieceplayer(hl));
            if(!dialogbox.exec()){
                // 对话框被关闭或点击了取消
                return false;
            }
            m_chessMap.setData(dialogbox.getValue(), st.x(), st.y());
        }
        // 处理王车易位标志
        char s = m_chessMap.getData(st.x(), st.y());
        if(s == 'K'){
            castleFlag[0] = false;
            castleFlag[1] = false;
        } else if(s == 'k'){
            castleFlag[2] = false;
            castleFlag[3] = false;
        }
        if((s == 'R' && st == QPoint(1, 1)) || (eg == QPoint(1, 1))){
            castleFlag[0] = false;
        }
        if((s == 'R' && st == QPoint(8, 1)) || (eg == QPoint(8, 1))){
            castleFlag[1] = false;
        }
        if((s == 'r' && st == QPoint(1, 8)) || (eg == QPoint(1, 8))){
            castleFlag[2] = false;
        }
        if((s == 'r' && st == QPoint(8, 8)) || (eg == QPoint(8, 8))){
            castleFlag[3] = false;
        }
        flagHistory.append(castleFlag);
        // 移动棋子
        m_chessMap.setData(m_chessMap.getData(st.x(),st.y()), eg.x(), eg.y());
        m_chessMap.setData(' ', st.x(), st.y());
        // 过路兵
        if(hl == 'E'){
            m_chessMap.setData(' ', eg.x(), 5);
        } else if(hl == 'e'){
            m_chessMap.setData(' ', eg.x(), 4);
        }
        m_boardhistory.append(m_chessMap);
        m_highLightMap.reset();
        emit chessMoved();
    } else if(hl == 'C' || hl == 'c'){
        return false;
    } else if(hl == 'Y' || hl == 'y'){
        // 王车易位
        QMessageBox castleBox;
        castleBox.setText("要进行王车易位吗？");
        castleBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        castleBox.setDefaultButton(QMessageBox::Yes);
        int ret = castleBox.exec();
        if(ret == QMessageBox::Cancel){
            return false;
        } else {
            if(eg == QPoint(3, 1)){
                m_chessMap.setData('K', 3, 1);
                m_chessMap.setData('R', 4, 1);
                m_chessMap.setData(' ', 1, 1);
                m_chessMap.setData(' ', 5, 1);
                castleFlag[0] = false;
                castleFlag[1] = false;
            } else if(eg == QPoint(7, 1)){
                m_chessMap.setData('K', 7, 1);
                m_chessMap.setData('R', 6, 1);
                m_chessMap.setData(' ', 8, 1);
                m_chessMap.setData(' ', 5, 1);
                castleFlag[0] = false;
                castleFlag[1] = false;
            } else if(eg == QPoint(3, 8)){
                m_chessMap.setData('k', 3, 8);
                m_chessMap.setData('r', 4, 8);
                m_chessMap.setData(' ', 1, 8);
                m_chessMap.setData(' ', 5, 8);
                castleFlag[2] = false;
                castleFlag[3] = false;
            } else if(eg == QPoint(7, 8)){
                m_chessMap.setData('k', 7, 8);
                m_chessMap.setData('r', 6, 8);
                m_chessMap.setData(' ', 8, 8);
                m_chessMap.setData(' ', 5, 8);
                castleFlag[2] = false;
                castleFlag[3] = false;
            } else {
                // 不应触发
                return false;
            }
            flagHistory.append(castleFlag);
            m_boardhistory.append(m_chessMap);
            m_highLightMap.reset();
            emit chessMoved();
        }
    }
    checkGameOver();
    return true;
}

bool ChessBoard::getGameOverCheckingFlag()
{
    return gameOverChecking;
}

int ChessBoard::pieceplayer(char piece)
{
    if('A' <= piece && piece <= 'Z'){
        // 白方棋子
        return 0;
    } else if('a' <= piece && piece <= 'z'){
        // 黑方棋子
        return 1;
    } else {
        return -1;
    }
}

void ChessBoard::highLight_P(int turn, int col, int row)
{
    BoardData chessMap = m_boardhistory.at(turn);
    // 向前走
    if(chessMap.getData(col, row+1) == ' '){
        m_highLightMap.setData('M', col, row+1);
        if(row == 2 && chessMap.getData(col, row+2) == ' '){
            // 起步可走两步
            m_highLightMap.setData('M', col, row+2);
        }
        // 升变
        if(row == 7){
            m_highLightMap.setData('R', col, row+1);
        }
    }
    // 吃黑子
    for(int i = -1; i <= 1; i+=2){
        if(pieceplayer(chessMap.getData(col+i, row+1)) == 1){
            m_highLightMap.setData('M', col+i, row+1);
            if(row == 7){
                m_highLightMap.setData('R', col+i, row+1);
            }
        }
    }
    // 过路兵
    if(row == 5){
        BoardData prevMap = m_boardhistory.at(turn-1);
        for(int i = -1; i<=1; i+=2){
            if(chessMap.getData(col+i, 5) == 'p' && chessMap.getData(col+i, 7) == ' ' &&
                    prevMap.getData(col+i, 5) == ' ' && prevMap.getData(col+i, 7) == 'p'){
                m_highLightMap.setData('E', col+i, 6);
            }
        }
    }
    verifyHighLightMap(0, chessMap, QPoint(col, row));
}

void ChessBoard::highLight_p(int turn, int col, int row)
{
    BoardData chessMap = m_boardhistory.at(turn);
    // 向前走
    if(chessMap.getData(col, row-1) == ' '){
        m_highLightMap.setData('m', col, row-1);
        if(row == 7 && chessMap.getData(col, row-2) == ' '){
            // 起步可走两步
            m_highLightMap.setData('m', col, row-2);
        }
        // 升变
        if(row == 2){
            m_highLightMap.setData('r', col, row-1);
        }
    }
    // 吃白子
    for(int i = -1; i <= 1; i+=2){
        if(pieceplayer(chessMap.getData(col+i, row-1)) == 0){
            m_highLightMap.setData('m', col+i, row-1);
            if(row == 2){
                m_highLightMap.setData('r', col+i, row-1);
            }
        }
    }
    // 过路兵
    if(row == 4){
        BoardData prevMap = m_boardhistory.at(turn-1);
        for(int i = -1; i<=1; i+=2){
            if(chessMap.getData(col+i, 4) == 'P' && chessMap.getData(col+i, 2) == ' ' &&
                    prevMap.getData(col+i, 4) == ' ' && prevMap.getData(col+i, 2) == 'P'){
                m_highLightMap.setData('e', col+i, 3);
            }
        }
    }
    verifyHighLightMap(1, chessMap, QPoint(col, row));
}

void ChessBoard::highLight_K(int turn, int col, int row)
{
    BoardData chessMap = m_boardhistory.at(turn);
    QVector<bool> t_flag = flagHistory.at(turn);
    // 走子，吃子
    for(int c = col-1; c <= col+1; c++){
        for(int r =row-1; r <= row+1; r++){
            if(c == col && r == row){
                continue;
            }
            char chess = chessMap.getData(c, r);
            if(chess == ' ' || pieceplayer(chess) == 1){
                m_highLightMap.setData('M', c, r);
            }
        }
    }
    // 王车易位
    if(col == 5 && row == 1 && t_flag[0] == true && chessMap.getData(1, 1) == 'R'
            && chessMap.getData(2, 1) == ' ' && chessMap.getData(3, 1) == ' ' && chessMap.getData(4, 1) == ' '){
        bool cf = true;
        for(int i = 3; i <= 5; i++){
            if(gridControlledByPlayer(1, chessMap, i, 1)){
                cf = false;
                break;
            }
        }
        if(cf){
            m_highLightMap.setData('Y', 3, 1);
        }
    }
    if(col == 5 && row == 1 && t_flag[1] == true && chessMap.getData(8, 1) == 'R'
            && chessMap.getData(7, 1) == ' ' && chessMap.getData(6, 1) == ' '){
        bool cf = true;
        for(int i = 5; i <= 7; i++){
            if(gridControlledByPlayer(1, chessMap, i, 1)){
                cf = false;
                break;
            }
        }
        if(cf){
            m_highLightMap.setData('Y', 7, 1);
        }
    }
    verifyHighLightMap(0, chessMap, QPoint(col, row));
}

void ChessBoard::highLight_k(int turn, int col, int row)
{
    BoardData chessMap = m_boardhistory.at(turn);
    QVector t_flag = flagHistory.at(turn);
    // 走子，吃子
    for(int c = col-1; c <= col+1; c++){
        for(int r =row-1; r <= row+1; r++){
            if(c == col && r == row){
                continue;
            }
            char chess = chessMap.getData(c, r);
            if(chess == ' ' || pieceplayer(chess) == 0){
                m_highLightMap.setData('m', c, r);
            }
        }
    }
    // 王车易位
    if(col == 5 && row == 8 && t_flag[2] == true && chessMap.getData(1, 8) == 'r'
            && chessMap.getData(2, 8) == ' ' && chessMap.getData(3, 8) == ' ' && chessMap.getData(4, 8) == ' '){
        bool cf = true;
        for(int i = 3; i <= 5; i++){
            if(gridControlledByPlayer(0, chessMap, i, 8)){
                cf = false;
                break;
            }
        }
        if(cf){
            m_highLightMap.setData('y', 3, 8);
        }
    }
    if(col == 5 && row == 8 && t_flag[3] == true && chessMap.getData(8, 8) == 'r'
            && chessMap.getData(7, 8) == ' ' && chessMap.getData(6, 8) == ' '){
        bool cf = true;
        for(int i = 5; i <= 7; i++){
            if(gridControlledByPlayer(0, chessMap, i, 8)){
                cf = false;
                break;
            }
        }
        if(cf){
            m_highLightMap.setData('y', 7, 8);
        }
    }
    verifyHighLightMap(1, chessMap, QPoint(col, row));
}

void ChessBoard::highLight_R(int turn, int col, int row)
{
    BoardData chessMap = m_boardhistory.at(turn);
    // 走子，吃子
    for(int i = col+1; i <= 8; i++){
        char chess = chessMap.getData(i, row);
        if(chess == ' '){
            m_highLightMap.setData('M', i, row);
        } else {
            if(pieceplayer(chess) == 1){
                m_highLightMap.setData('M', i, row);
                break;
            } else {
                break;
            }
        }
    }
    for(int i = col-1; i >= 0; i--){
        char chess = chessMap.getData(i, row);
        if(chess == ' '){
            m_highLightMap.setData('M', i, row);
        } else {
            if(pieceplayer(chess) == 1){
                m_highLightMap.setData('M', i, row);
                break;
            } else {
                break;
            }
        }
    }
    for(int j = row+1; j <= 8; j++){
        char chess = chessMap.getData(col, j);
        if(chess == ' '){
            m_highLightMap.setData('M', col, j);
        } else {
            if(pieceplayer(chess) == 1){
                m_highLightMap.setData('M', col, j);
                break;
            } else {
                break;
            }
        }
    }
    for(int j = row-1; j >= 0; j--){
        char chess = chessMap.getData(col, j);
        if(chess == ' '){
            m_highLightMap.setData('M', col, j);
        } else {
            if(pieceplayer(chess) == 1){
                m_highLightMap.setData('M', col, j);
                break;
            } else {
                break;
            }
        }
    }
    verifyHighLightMap(0, chessMap, QPoint(col, row));
}

void ChessBoard::highLight_r(int turn, int col, int row)
{
    BoardData chessMap = m_boardhistory.at(turn);
    // 走子，吃子
    for(int i = col+1; i <= 8; i++){
        char chess = chessMap.getData(i, row);
        if(chess == ' '){
            m_highLightMap.setData('m', i, row);
        } else {
            if(pieceplayer(chess) == 0){
                m_highLightMap.setData('m', i, row);
                break;
            } else {
                break;
            }
        }
    }
    for(int i = col-1; i >= 0; i--){
        char chess = chessMap.getData(i, row);
        if(chess == ' '){
            m_highLightMap.setData('m', i, row);
        } else {
            if(pieceplayer(chess) == 0){
                m_highLightMap.setData('m', i, row);
                break;
            } else {
                break;
            }
        }
    }
    for(int j = row+1; j <= 8; j++){
        char chess = chessMap.getData(col, j);
        if(chess == ' '){
            m_highLightMap.setData('m', col, j);
        } else {
            if(pieceplayer(chess) == 0){
                m_highLightMap.setData('m', col, j);
                break;
            } else {
                break;
            }
        }
    }
    for(int j = row-1; j >= 0; j--){
        char chess = chessMap.getData(col, j);
        if(chess == ' '){
            m_highLightMap.setData('m', col, j);
        } else {
            if(pieceplayer(chess) == 0){
                m_highLightMap.setData('m', col, j);
                break;
            } else {
                break;
            }
        }
    }
    verifyHighLightMap(1, chessMap, QPoint(col, row));
}

void ChessBoard::highLight_B(int turn, int col, int row)
{
    BoardData chessMap = m_boardhistory.at(turn);
    for(int i = col+1, j = row+1; (i <= 8 && j <= 8); i++,j++){
        char chess = chessMap.getData(i, j);
        if(chess == ' '){
            m_highLightMap.setData('M', i, j);
        } else {
            if(pieceplayer(chess) == 1){
                m_highLightMap.setData('M', i, j);
                break;
            } else {
                break;
            }
        }
    }
    for(int i = col-1, j = row+1; (i >= 0 && j <= 8); i--,j++){
        char chess = chessMap.getData(i, j);
        if(chess == ' '){
            m_highLightMap.setData('M', i, j);
        } else {
            if(pieceplayer(chess) == 1){
                m_highLightMap.setData('M', i, j);
                break;
            } else {
                break;
            }
        }
    }
    for(int i = col+1, j = row-1; (i <= 8 && j >= 0); i++,j--){
        char chess = chessMap.getData(i, j);
        if(chess == ' '){
            m_highLightMap.setData('M', i, j);
        } else {
            if(pieceplayer(chess) == 1){
                m_highLightMap.setData('M', i, j);
                break;
            } else {
                break;
            }
        }
    }
    for(int i = col-1, j = row-1; (i >= 0 && j >= 0); i--,j--){
        char chess = chessMap.getData(i, j);
        if(chess == ' '){
            m_highLightMap.setData('M', i, j);
        } else {
            if(pieceplayer(chess) == 1){
                m_highLightMap.setData('M', i, j);
                break;
            } else {
                break;
            }
        }
    }
    verifyHighLightMap(0, chessMap, QPoint(col, row));
}

void ChessBoard::highLight_b(int turn, int col, int row)
{
    BoardData chessMap = m_boardhistory.at(turn);
    for(int i = col+1, j = row+1; (i <= 8 && j <= 8); i++,j++){
        char chess = chessMap.getData(i, j);
        if(chess == ' '){
            m_highLightMap.setData('m', i, j);
        } else {
            if(pieceplayer(chess) == 0){
                m_highLightMap.setData('m', i, j);
                break;
            } else {
                break;
            }
        }
    }
    for(int i = col-1, j = row+1; (i >= 0 && j <= 8); i--,j++){
        char chess = chessMap.getData(i, j);
        if(chess == ' '){
            m_highLightMap.setData('m', i, j);
        } else {
            if(pieceplayer(chess) == 0){
                m_highLightMap.setData('m', i, j);
                break;
            } else {
                break;
            }
        }
    }
    for(int i = col+1, j = row-1; (i <= 8 && j >= 0); i++,j--){
        char chess = chessMap.getData(i, j);
        if(chess == ' '){
            m_highLightMap.setData('m', i, j);
        } else {
            if(pieceplayer(chess) == 0){
                m_highLightMap.setData('m', i, j);
                break;
            } else {
                break;
            }
        }
    }
    for(int i = col-1, j = row-1; (i >= 0 && j >= 0); i--,j--){
        char chess = chessMap.getData(i, j);
        if(chess == ' '){
            m_highLightMap.setData('m', i, j);
        } else {
            if(pieceplayer(chess) == 0){
                m_highLightMap.setData('m', i, j);
                break;
            } else {
                break;
            }
        }
    }
    verifyHighLightMap(1, chessMap, QPoint(col, row));
}

void ChessBoard::highLight_Q(int turn, int col, int row)
{
    highLight_R(turn, col, row);
    highLight_B(turn, col, row);
}

void ChessBoard::highLight_q(int turn, int col, int row)
{
    highLight_r(turn, col, row);
    highLight_b(turn, col, row);
}

void ChessBoard::highLight_N(int turn, int col, int row)
{
    BoardData chessMap = m_boardhistory.at(turn);
    auto abs = [](int x) -> int{return x>0 ? x: -x;};
    for(int o_c = -2; o_c <= 2; o_c++){
        for(int o_r = -2; o_r <= 2; o_r++){
            if(abs(o_c)+abs(o_r) == 3){
                char chess = chessMap.getData(col + o_c, row + o_r);
                if(chess == ' ' || pieceplayer(chess) == 1){
                    m_highLightMap.setData('M', col + o_c, row + o_r);
                }
            }
        }
    }
    verifyHighLightMap(0, chessMap, QPoint(col, row));
}

void ChessBoard::highLight_n(int turn, int col, int row)
{
    BoardData chessMap = m_boardhistory.at(turn);
    auto abs = [](int x) -> int{return x>0 ? x: -x;};
    for(int o_c = -2; o_c <= 2; o_c++){
        for(int o_r = -2; o_r <= 2; o_r++){
            if(abs(o_c)+abs(o_r) == 3){
                char chess = chessMap.getData(col + o_c, row + o_r);
                if(chess == ' ' || pieceplayer(chess) == 0){
                    m_highLightMap.setData('m', col + o_c, row + o_r);
                }
            }
        }
    }
    verifyHighLightMap(1, chessMap, QPoint(col, row));
}

bool ChessBoard::gridControlledByPlayer(int player, BoardData chessBoard, int c, int r)
{
    if(player == 0){
        if(chessBoard.getData(c+1,r+2) == 'N' || chessBoard.getData(c+1,r-2) == 'N' || chessBoard.getData(c+2,r+1) == 'N'
                || chessBoard.getData(c+2,r-1) == 'N' || chessBoard.getData(c-1,r+2) == 'N' || chessBoard.getData(c-1,r-2) == 'N'
                || chessBoard.getData(c-2,r+1) == 'N' || chessBoard.getData(c-2,r-1) == 'N'){
            // 检查马
            qDebug() << "controlled by N";
            return true;
        }
        if(chessBoard.getData(c-1,r-1) == 'P' || chessBoard.getData(c+1,r-1) == 'P'){
            // 检查兵
            qDebug() << "controlled by P";
            return true;
        }
        // 检查王
        for(int i = c-1; i <= c+1; i++){
            for(int j = r-1; j <= r+1; j++){
                if(chessBoard.getData(i ,j) == 'K'){
                    qDebug() << "controlled by K";
                    return true;
                }
            }
        }
        // 检查横纵
        for(int i = c+1; i <= 8; i++){
            char chess = chessBoard.getData(i, r);
            if(chess != ' ' && chess != '#'){
                if(chess == 'R' || chess == 'Q'){
                    qDebug() << "controlled by R or Q at 1";
                    return true;
                } else {
                    break;
                }
            }
        }
        for(int i = c-1; i >= 1; i--){
            char chess = chessBoard.getData(i, r);
            if(chess != ' ' && chess != '#'){
                if(chess == 'R' || chess == 'Q'){
                    qDebug() << "controlled by R or Q at 2";
                    return true;
                } else {
                    break;
                }
            }
        }
        for(int j = r+1; j <= 8; j++){
            char chess = chessBoard.getData(c, j);
            if(chess != ' ' && chess != '#'){
                if(chess == 'R' || chess == 'Q'){
                    qDebug() << "controlled by R or Q at 3";
                    return true;
                } else {
                    break;
                }
            }
        }
        for(int j = r-1; j >= 0; j--){
            char chess = chessBoard.getData(c, j);
            if(chess != ' ' && chess != '#'){
                if(chess == 'R' || chess == 'Q'){
                    qDebug() << "controlled by R or Q at 4";
                    return true;
                } else {
                    break;
                }
            }
        }
        // 检查斜向
        for(int i = c+1, j = r+1; (i <= 8 && j <= 8); i++,j++){
            char chess = chessBoard.getData(i, j);
            if(chess != ' ' && chess != '#'){
                if(chess == 'B' || chess == 'Q'){
                    qDebug() << "controlled by B or Q at 1";
                    return true;
                } else {
                    break;
                }
            }
        }
        for(int i = c-1, j = r+1; (i >= 0 && j <= 8); i--,j++){
            char chess = chessBoard.getData(i, j);
            if(chess != ' ' && chess != '#'){
                if(chess == 'B' || chess == 'Q'){
                    qDebug() << "controlled by B or Q at 2";
                    return true;
                } else {
                    break;
                }
            }
        }
        for(int i = c+1, j = r-1; (i <= 8 && j >= 0); i++,j--){
            char chess = chessBoard.getData(i, j);
            if(chess != ' ' && chess != '#'){
                if(chess == 'B' || chess == 'Q'){
                    qDebug() << "controlled by B or Q at 3";
                    return true;
                } else {
                    break;
                }
            }
        }
        for(int i = c-1, j = r-1; (i >= 0 && j >= 0); i--,j--){
            char chess = chessBoard.getData(i, j);
            if(chess != ' ' && chess != '#'){
                if(chess == 'B' || chess == 'Q'){
                    qDebug() << "controlled by B or Q at 4";
                    return true;
                } else {
                    break;
                }
            }
        }
    } else if(player == 1){
        if(chessBoard.getData(c+1,r+2) == 'n' || chessBoard.getData(c+1,r-2) == 'n' || chessBoard.getData(c+2,r+1) == 'n'
                || chessBoard.getData(c+2,r-1) == 'n' || chessBoard.getData(c-1,r+2) == 'n' || chessBoard.getData(c-1,r-2) == 'n'
                || chessBoard.getData(c-2,r+1) == 'n' || chessBoard.getData(c-2,r-1) == 'n'){
            // 检查马
            qDebug() << "controlled by n";
            return true;
        }
        if(chessBoard.getData(c-1,r+1) == 'p' || chessBoard.getData(c+1,r+1) == 'p'){
            // 检查兵
            qDebug() << "controlled by p";
            return true;
        }
        // 检查王
        for(int i = c-1; i <= c+1; i++){
            for(int j = r-1; j <= r+1; j++){
                if(chessBoard.getData(i ,j) == 'k'){
                    qDebug() << "controlled by k";
                    return true;
                }
            }
        }
        // 检查横纵
        for(int i = c+1; i <= 8; i++){
            char chess = chessBoard.getData(i, r);
            if(chess != ' ' && chess != '#'){
                if(chess == 'r' || chess == 'q'){
                    qDebug() << "controlled by r or q at 1";
                    return true;
                } else {
                    break;
                }
            }
        }
        for(int i = c-1; i >= 1; i--){
            char chess = chessBoard.getData(i, r);
            if(chess != ' ' && chess != '#'){
                if(chess == 'r' || chess == 'q'){
                    qDebug() << "controlled by r or q at 2";
                    return true;
                } else {
                    break;
                }
            }
        }
        for(int j = r+1; j <= 8; j++){
            char chess = chessBoard.getData(c, j);
            if(chess != ' ' && chess != '#'){
                if(chess == 'r' || chess == 'q'){
                    qDebug() << "controlled by r or q at 3";
                    return true;
                } else {
                    break;
                }
            }
        }
        for(int j = r-1; j >= 0; j--){
            char chess = chessBoard.getData(c, j);
            if(chess != ' ' && chess != '#'){
                if(chess == 'r' || chess == 'q'){
                    qDebug() << "controlled by r or q at 4";
                    return true;
                } else {
                    break;
                }
            }
        }
        // 检查斜向
        for(int i = c+1, j = r+1; (i <= 8 && j <= 8); i++,j++){
            char chess = chessBoard.getData(i, j);
            if(chess != ' ' && chess != '#'){
                if(chess == 'b' || chess == 'q'){
                    qDebug() << "controlled by b or q at 1";
                    return true;
                } else {
                    break;
                }
            }
        }
        for(int i = c-1, j = r+1; (i >= 0 && j <= 8); i--,j++){
            char chess = chessBoard.getData(i, j);
            if(chess != ' ' && chess != '#'){
                if(chess == 'b' || chess == 'q'){
                    qDebug() << "controlled by b or q at 2";
                    return true;
                } else {
                    break;
                }
            }
        }
        for(int i = c+1, j = r-1; (i <= 8 && j >= 0); i++,j--){
            char chess = chessBoard.getData(i, j);
            if(chess != ' ' && chess != '#'){
                if(chess == 'b' || chess == 'q'){
                    qDebug() << "controlled by b or q at 3";
                    return true;
                } else {
                    break;
                }
            }
        }
        for(int i = c-1, j = r-1; (i >= 0 && j >= 0); i--,j--){
            char chess = chessBoard.getData(i, j);
            if(chess != ' ' && chess != '#'){
                if(chess == 'b' || chess == 'q'){
                    qDebug() << "controlled by b or q at 4";
                    return true;
                } else {
                    break;
                }
            }
        }
    }
    return false;
}

void ChessBoard::checkGameOver()
{
    gameOverChecking = true;
    // 检查王是否被吃，正常情况下不应出现王被吃的情况
    int k_e = checkKingExist();
    if(k_e <= 0){
        // 王被吃，为非正常情况
        emit gameOver(k_e);
        gameOverChecking = false;
        return;
    }
    // 检查是否无合法移动
    if(checkNoLegalMove()){
        // 无合法移动，检查是将死（checkmate）还是无子可动（stalemate）
        int player = 1 - (m_boardhistory.length() % 2);
        int checkflag = checkCheck(player, m_chessMap);
        if(checkflag == 1){
            // checkmate!
            if(player == 1){
                // 白方将死黑方（白胜）
                emit gameOver(1);
            } else {
                // 黑方将死白方（黑胜）
                emit gameOver(2);
            }
        } else if(checkflag == 0){
            // stalemate
            if(player == 0){
                // 白方无子可动（平局）
                emit gameOver(3);
            } else {
                // 黑方无子可动（平局）
                emit gameOver(4);
            }
        } else{
            emit gameOver(-3); // unknown error
        }
    }
    gameOverChecking = false;
    return;
}

int ChessBoard::checkKingExist()
{
    bool K_exist = false, k_exist = false;
    for(int c = 1; c <= 8; c++){
        for(int r = 1; r <= 8; r++){
            char chess = m_chessMap.getData(c ,r);
            if(chess == 'K'){
                K_exist = true;
            } else if(chess == 'k'){
                k_exist = true;
            }
        }
    }
    if(K_exist && k_exist){
        return 1;
    } else if(K_exist){
        return 0;
    } else if(k_exist){
        return -1;
    } else {
        return -2;
    }
}

bool ChessBoard::checkNoLegalMove()
{
    int player = 1 - (m_boardhistory.length() % 2);
    int turn = m_boardhistory.length() - 1;
    for(int c = 1; c <= 8; c++){
        for(int r = 1; r <=8; r++){
            char chess = m_chessMap.getData(c, r);
            if(pieceplayer(chess) == player){
                updateHighLightMap(QPoint(c, r), turn);
                // 依次检查是否有移动标志
                for(int i = 0; i <= 8; i++){
                    for(int j = 0; j <= 8; j++){
                        char hl = m_highLightMap.getData(i, j);
                        if(hl == 'M' || hl == 'm' || hl == 'R' || hl == 'r' ||
                                hl == 'E' || hl == 'e' || hl == 'Y' || hl == 'y'){
                            // 有合法移动
                            m_highLightMap.reset();
                            return false;
                        }
                    }
                }
            }
        }
    }
    m_highLightMap.reset();
    return true;
}

int ChessBoard::checkCheck(int player, BoardData chessMap)
{
    // 检查player的king是否被将军
    QPoint k_pos = QPoint();
    for(int c = 1; c <= 8; c++){
        for(int r = 1; r <= 8; r++){
            if(chessMap.getData(c, r) == 'K' && player == 0){
                k_pos = QPoint(c ,r);
            } else if(chessMap.getData(c, r) == 'k' && player == 1){
                k_pos = QPoint(c,r);
            }
        }
    }
    if(k_pos.isNull()){
        // 发生错误，未找到王
        return -1;
    }
    if(gridControlledByPlayer(1-player, chessMap, k_pos.x(), k_pos.y())){
        return 1;
    } else {
        return 0;
    }
}

void ChessBoard::verifyHighLightMap(int player, BoardData chessMap, QPoint eg)
{
    // 检查初步绘制的highlight中各移动步是否导致送将（player方的王是否在移棋后被将军）
    for(int c = 1; c <= 8; c++){
        for(int r = 1; r <= 8; r++){
            char hl = m_highLightMap.getData(c, r);
            if(hl == 'M' || hl == 'm' || hl == 'E' || hl == 'e' || hl == 'R' || hl == 'r'){
                BoardData tmpMap = BoardData(chessMap);
                tmpMap.setData(tmpMap.getData(eg.x(),eg.y()), c, r);
                tmpMap.setData(' ', eg.x(), eg.y());
                if(hl == 'E'){
                    tmpMap.setData(' ', c, 5);
                } else if(hl == 'e'){
                    tmpMap.setData(' ', c, 4);
                }
                int flag = checkCheck(player, tmpMap);
                if(flag == 1){
                    // 该移动会导致送将
                    if(player == 0){
                        m_highLightMap.setData('C', c, r);
                    } else if(player == 1){
                        m_highLightMap.setData('c', c, r);
                    }
                }
            }
        }
    }
}

BoardData::BoardData()
{
    for(int c = 1; c < 9; c++){
        for(int r = 1; r < 9; r++){
            m_data[c-1][r-1] = ' ';
        }
    }
}

char BoardData::getData(int col, int row)
{
    if(col < 1 || col > 8 || row < 1 || row > 8){
        return '#';
    }
    return m_data[col-1][row-1];
}

void BoardData::setData(char ch, int col, int row)
{
    if(col < 1 || col > 8 || row < 1 || row > 8){
        return;
    }
    m_data[col-1][row-1] = ch;
}

void BoardData::reset()
{
    for(int c = 1; c < 9; c++){
        for(int r = 1; r < 9; r++){
            m_data[c-1][r-1] = ' ';
        }
    }
}
