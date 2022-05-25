#include "chessview.h"
#include <QPainter>
#include <QDebug>
#include <QtMath>

ChessView::ChessView(QWidget *parent)
    : QWidget{parent}
{
    chessChosed = false;
    dragOn = false;
    m_chosedPiece = QPoint();
    m_drawturn = 0;
    gameIsOver = false;
    isShowHistory = false;
    m_piece.insert('K', QIcon(":/img/Chess_klt45.svg"));
    m_piece.insert('Q', QIcon(":/img/Chess_qlt45.svg"));
    m_piece.insert('R', QIcon(":/img/Chess_rlt45.svg"));
    m_piece.insert('B', QIcon(":/img/Chess_blt45.svg"));
    m_piece.insert('N', QIcon(":/img/Chess_nlt45.svg"));
    m_piece.insert('P', QIcon(":/img/Chess_plt45.svg"));
    m_piece.insert('k', QIcon(":/img/Chess_kdt45.svg"));
    m_piece.insert('q', QIcon(":/img/Chess_qdt45.svg"));
    m_piece.insert('r', QIcon(":/img/Chess_rdt45.svg"));
    m_piece.insert('b', QIcon(":/img/Chess_bdt45.svg"));
    m_piece.insert('n', QIcon(":/img/Chess_ndt45.svg"));
    m_piece.insert('p', QIcon(":/img/Chess_pdt45.svg"));
}

void ChessView::setBoard(ChessBoard *chessboard){
    m_board = chessboard;
}

void ChessView::nextTurn(int new_turn)
{
    if(m_maxturn != new_turn){
        m_maxturn = new_turn;
        m_drawturn = m_maxturn;
        update();
    }
}

void ChessView::changeShowTurn(int show_turn)
{
    if(m_drawturn != show_turn){
        m_drawturn = show_turn;
        m_board->resetHighLightMap();
        chessChosed = false;
        m_chosedPiece = QPoint();
        if(m_drawturn != m_maxturn){
            isShowHistory = true;
        } else {
            isShowHistory = false;
        }
        update();
    }
}

void ChessView::setGameOverView()
{
    if(!gameIsOver){
        gameIsOver = true;
        update();
    }
}

void ChessView::drawGrid(QPainter *painter)
{
    painter -> setPen(Qt::NoPen);
    for(int c = 1; c <= 8; c++){
        for(int r = 1; r <=8; r++){
            if((c + r) % 2){
                painter -> setBrush(Qt::lightGray);
                painter -> drawRect(gridRect(c, r));
            } else {
                painter -> setBrush(Qt::gray);
                painter -> drawRect(gridRect(c, r));
            }
        }
    }
}

void ChessView::drawPieces(QPainter *painter, BoardData *data)
{
    for(int c = 1; c < 9; c++){
        for(int r = 1; r < 9; r++){
            char ch = data->getData(c, r);
            if(ch != ' '){
                if(!dragOn || QPoint(c, r) != m_chosedPiece){
                    QIcon icon = m_piece.value(ch);
                    icon.paint(painter, gridRect(c, r));
                }
            }
        }
    }
}

void ChessView::drawHighLight(QPainter *painter, BoardData *highlightmap)
{
    painter -> setPen(Qt::NoPen);
    QMap<char,QColor> colors;
    if(m_drawturn % 2){
        // 黑先
        colors = {
            {'m', QColor(102, 255, 102, 127)},
            {'a', QColor(102, 255, 102, 127)},
            {'e', QColor(102, 255, 102, 127)},
            {'r', QColor(255, 255, 102, 127)},
            {'c', QColor(127, 0, 255, 127)},
            {'y', QColor(255, 255, 102, 127)},
            {'M', QColor(255, 102, 102, 127)},
            {'A', QColor(255, 102, 102, 127)},
            {'E', QColor(255, 102, 102, 127)},
            {'R', QColor(255, 102, 102, 127)},
            {'C', QColor(255, 102, 102, 127)},
            {'Y', QColor(255, 102, 102, 127)}
        };
    } else {
        // 白先
        colors = {
            {'M', QColor(102, 255, 102, 127)},
            {'A', QColor(102, 255, 102, 127)},
            {'E', QColor(102, 255, 102, 127)},
            {'R', QColor(255, 255, 102, 127)},
            {'C', QColor(127, 0, 255, 127)},
            {'Y', QColor(255, 255, 102, 127)},
            {'m', QColor(255, 102, 102, 127)},
            {'a', QColor(255, 102, 102, 127)},
            {'e', QColor(255, 102, 102, 127)},
            {'r', QColor(255, 102, 102, 127)},
            {'c', QColor(255, 102, 102, 127)},
            {'y', QColor(255, 102, 102, 127)}
        };
    }
    for(int c = 1; c < 9; c++){
        for(int r = 1; r < 9; r++){
            char hl = highlightmap->getData(c, r);
            if(colors.contains(hl)){
                painter->setBrush(colors.value(hl));
                painter->drawRect(gridRect(c, r));
            }
        }
    }
}

void ChessView::drawDrag(QPainter *painter, BoardData *data)
{
    char ch = data->getData(m_chosedPiece.x(), m_chosedPiece.y());
    QIcon icon = m_piece.value(ch);
    icon.paint(painter,
               QRect(mousePoint-QPoint(this->width()/2, this->height()/2)-QPoint(gs/2, gs/2),
                     QSize(gs, gs)));
}


QRect ChessView::gridRect(int column, int row)
{
    if(column < 1 || column > 8 || row < 1 || row > 8){
        return QRect();
    }
    return QRect((column -5)*gs, (4 - row)*gs, gs, gs);
}

QPoint ChessView::eventGrid(QPoint eventpoint)
{
    double x = eventpoint.x() - this->width()/2;
    double y = eventpoint.y() - this->height()/2;
    int col = qFloor(x / gs) + 5;
    int row = -qFloor(y / gs) + 4;
    if(col < 1 || col > 8 || row < 1 || row > 8){
        return QPoint();
    } else {
        return QPoint(col, row);
    }
}

int ChessView::pieceplayer(char piece)
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

void ChessView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.translate(this->width()/2, this->height()/2);

    painter.save();
    drawGrid(&painter);
    painter.restore();

    painter.save();
    drawHighLight(&painter, m_board->getHighLightMap());
    painter.restore();

    painter.save();
    drawPieces(&painter, m_board->getBoardData(m_drawturn));
    painter.restore();

    if(dragOn){
        painter.save();
        drawDrag(&painter, m_board->getBoardData(m_drawturn));
        painter.restore();
    }

}

void ChessView::mousePressEvent(QMouseEvent *event)
{
    if(m_board->getGameOverCheckingFlag()){
        // 游戏结束检查中，为保证安全不接收新的输入
        return;
    }
    QPoint eg = eventGrid(event->pos());
    BoardData *chessMap = m_board->getBoardData(m_drawturn);
    BoardData *highLightMap = m_board->getHighLightMap();
    char piece = '#';
    char hl = '#';
    if(!eg.isNull()){
        piece = chessMap->getData(eg.x(),eg.y());
        hl = highLightMap->getData(eg.x(),eg.y());
    }

    if(!chessChosed || gameIsOver || isShowHistory){
        // 没有己方棋子已被选中，此时不启动拖动，但切换展示
        if(piece == ' ' || piece == '#'){
            // 点击了空格子或棋盘外
            m_board->resetHighLightMap();
            update();
        } else {
            // 点击了棋子
            m_board->updateHighLightMap(eg, m_drawturn);
            update();
            if(pieceplayer(piece) == m_drawturn % 2) {
                // 激活选中
                chessChosed = true;
                m_chosedPiece = eg;
            }
        }
    } else {
        if((piece == ' ' || piece == '#') && (hl == ' ' || hl == '#')){
            // 空格子且无highlight
            m_board->resetHighLightMap();
            chessChosed = false;
            m_chosedPiece = QPoint();
            update();
        } else if (hl != ' ' && hl != '#' && hl != 'A' && hl != 'a'){
            // 是highlight，走棋
            bool moved = m_board->moveChess(m_chosedPiece, eg);
            if(moved){
                chessChosed = false;
                m_chosedPiece = QPoint();
            } else {
                update();
            }

        } else if (pieceplayer(piece) == (1 - (m_drawturn % 2))){
            // 选择了对方棋，但不能走棋
            chessChosed = false;
            m_chosedPiece = QPoint();
            m_board->updateHighLightMap(eg, m_drawturn);
            update();
        } else if (pieceplayer(piece) == m_drawturn % 2 && eg != m_chosedPiece){
            // 选择我方棋，但和之前选的不一致，不启用拖动
            m_board->updateHighLightMap(eg, m_drawturn);
            update();
            m_chosedPiece = eg;
        } else if(eg == m_chosedPiece){
            // 启用拖动
            dragOn = true;
        }
    }
}

void ChessView::mouseReleaseEvent(QMouseEvent *event)
{
    if(dragOn){
        // 关闭拖动
        dragOn = false;

        QPoint eg = eventGrid(event->pos());
        // BoardData *chessMap = m_board->getBoardData(m_drawturn);
        BoardData *highLightMap = m_board->getHighLightMap();
        char piece = '#';
        char hl = '#';
        if(!eg.isNull()){
            // piece = chessMap->getData(eg.x(),eg.y());
            hl = highLightMap->getData(eg.x(),eg.y());
        }
        if(hl != ' ' && hl != '#' && hl != 'A' && hl != 'a'){
            // 走棋
            bool moved =  m_board->moveChess(m_chosedPiece, eg);
            if(moved){
                chessChosed = false;
                m_chosedPiece = QPoint();
            } else{
                update();
            }
        } else {
            update();
        }
    }

}

void ChessView::mouseMoveEvent(QMouseEvent *event)
{
    if(dragOn){
        mousePoint = event->pos();
        // qDebug() << mousePoint;
        update();
    }
}

void ChessView::resizeEvent(QResizeEvent *event)
{
    // qDebug() << width() << height();
    gs = std::min(width(), height()) / 8;
}

QSize ChessView::sizeHint() const
{
    return QSize(700,700);
}

QSize ChessView::minimumSizeHint() const
{
    return QSize(500,500);
}


