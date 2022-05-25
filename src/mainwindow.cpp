#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_board = new ChessBoard(this);
    m_logic = new ChessLogic(this);
    ui->chesswidget->setBoard(m_board);
    ui->turnSpinBox->setMaximum(0);

    connectSignalToSlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showResultMessageBox(int winner, int way)
{
    QMessageBox msgBox;
    if(way == -1){
        // 王被杀，不应出现
        if(winner == 0){
            msgBox.setText("黑王被杀，白方获胜");
        } else if(winner == 1){
            msgBox.setText("白王被杀，黑方获胜");
        } else{
            msgBox.setText("平局");
        }
    } else if(way == 0){
        // 将死
        msgBox.setWindowTitle("将死");
        if(winner == 0){
            msgBox.setText("白方将死黑王，白方获胜");
        } else {
            msgBox.setText("黑方将死白王，黑方获胜");
        }
    } else if(way == 1){
        // 无子可动（非照将），平局
        msgBox.setWindowTitle("无子可动");
        if(winner == 2){
            msgBox.setText("白方无子可动，平局");
        } else {
            msgBox.setText("黑方无子可动，平局");
        }
    } else if(way == 2){
        // 双方协商后平局
        msgBox.setWindowTitle("协商平局");
        msgBox.setText("双方同意平局");
    } else if(way == 3){
        // 投子认负
        msgBox.setWindowTitle("投子认负");
        if(winner == 0){
            msgBox.setText("黑方投子认负，白方获胜");
        } else {
            msgBox.setText("白方投子认负，黑方获胜");
        }
    }
    msgBox.exec();
}

void MainWindow::newTurn(int new_turn)
{
    // 处理turnSpinBox
    ui->turnSpinBox->setMaximum(new_turn);
    ui->turnSpinBox->blockSignals(true);
    ui->turnSpinBox->setValue(new_turn);
    ui->turnSpinBox->blockSignals(false);
    // TurnLable
    ui->turnLabel->setText("已走步数："+QString::number(new_turn));
}

void MainWindow::reset()
{
    // qDebug() << "here reset";
    QSize oldSize = this->size();
    delete ui;
    delete m_board;
    delete m_logic;

    ui = new Ui::MainWindow;
    ui->setupUi(this);
    this->resize(oldSize);
    m_board = new ChessBoard(this);
    m_logic = new ChessLogic(this);
    ui->chesswidget->setBoard(m_board);
    ui->turnSpinBox->setMaximum(0);

    connectSignalToSlot();
}

void MainWindow::connectSignalToSlot()
{
    connect(m_board, &ChessBoard::chessMoved, m_logic, &ChessLogic::nextTurn);
    connect(m_logic, &ChessLogic::TurnChanged, ui->chesswidget, &ChessView::nextTurn);
    connect(m_board, &ChessBoard::gameOver, m_logic, &ChessLogic::gameOver);
    connect(m_logic, &ChessLogic::ShowResult, ui->chesswidget, &ChessView::setGameOverView);
    connect(m_logic, &ChessLogic::TurnChanged, this, &MainWindow::newTurn);
    connect(ui->turnSpinBox, SIGNAL(valueChanged(int)), ui->chesswidget, SLOT(changeShowTurn(int)));
    connect(ui->showTurnResetButton, &QPushButton::clicked, m_logic, &ChessLogic::showTurnResetButtonClicked);
    connect(m_logic, &ChessLogic::ResetTurnSpinBox, ui->turnSpinBox, &QSpinBox::setValue);
    connect(m_logic, &ChessLogic::TurnChanged, ui->progressWidget, &ChessProgressWidget::newTurn);
    connect(ui->progressWidget, &ChessProgressWidget::resetGame, this, &MainWindow::reset);
    connect(ui->progressWidget, &ChessProgressWidget::gameOver, m_logic, &ChessLogic::gameOver);
    connect(m_logic, &ChessLogic::ShowResult, ui->progressWidget, &ChessProgressWidget::setGameOverView);
    connect(m_logic, &ChessLogic::ShowResult, this, &MainWindow::showResultMessageBox);
}

