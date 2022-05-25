#include "promotiondialog.h"
#include "qlayout.h"
#include "ui_promotiondialog.h"

PromotionDialog::PromotionDialog(int player, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PromotionDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(478, 263);

    m_player = player;

    if(player == 0){
        ui->pushButton_1->setIcon(QIcon(":/img/Chess_qlt45.svg"));
        ui->pushButton_2->setIcon(QIcon(":/img/Chess_rlt45.svg"));
        ui->pushButton_3->setIcon(QIcon(":/img/Chess_nlt45.svg"));
        ui->pushButton_4->setIcon(QIcon(":/img/Chess_blt45.svg"));
        val = 'Q';
    } else {
        ui->pushButton_1->setIcon(QIcon(":/img/Chess_qdt45.svg"));
        ui->pushButton_2->setIcon(QIcon(":/img/Chess_rdt45.svg"));
        ui->pushButton_3->setIcon(QIcon(":/img/Chess_ndt45.svg"));
        ui->pushButton_4->setIcon(QIcon(":/img/Chess_bdt45.svg"));
        val = 'q';
    }
    ui->pushButton_1->setIconSize(QSize(100, 100));
    ui->pushButton_2->setIconSize(QSize(100, 100));
    ui->pushButton_3->setIconSize(QSize(100, 100));
    ui->pushButton_4->setIconSize(QSize(100, 100));

    connect(ui->pushButton_1, &QPushButton::clicked, this, &PromotionDialog::b1_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &PromotionDialog::b2_clicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &PromotionDialog::b3_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &PromotionDialog::b4_clicked);
}

PromotionDialog::~PromotionDialog()
{
    delete ui;
}

char PromotionDialog::getValue()
{
    return val;
}

void PromotionDialog::b1_clicked()
{
    if(m_player == 0){
        val = 'Q';
    } else {
        val = 'q';
    }
}

void PromotionDialog::b2_clicked()
{
    if(m_player == 0){
        val = 'R';
    } else {
        val = 'r';
    }
}

void PromotionDialog::b3_clicked()
{
    if(m_player == 0){
        val = 'N';
    } else {
        val = 'n';
    }
}

void PromotionDialog::b4_clicked()
{
    if(m_player == 0){
        val = 'B';
    } else {
        val = 'b';
    }
}
