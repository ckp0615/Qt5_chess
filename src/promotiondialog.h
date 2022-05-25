#ifndef PROMOTIONDIALOG_H
#define PROMOTIONDIALOG_H

#include <QDialog>

namespace Ui {
class PromotionDialog;
}

class PromotionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PromotionDialog(int player = 0, QWidget *parent = nullptr);
    ~PromotionDialog();
    char getValue();

private slots:
    void b1_clicked();
    void b2_clicked();
    void b3_clicked();
    void b4_clicked();

private:
    Ui::PromotionDialog *ui;
    int m_player;
    char val;
};

#endif // PROMOTIONDIALOG_H
