#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chessboard.h"
#include "chesslogic.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showResultMessageBox(int winner, int way);
    void newTurn(int new_turn);
    void reset();

private:
    void connectSignalToSlot();
    Ui::MainWindow *ui;
    ChessBoard *m_board;
    ChessLogic *m_logic;
};
#endif // MAINWINDOW_H
