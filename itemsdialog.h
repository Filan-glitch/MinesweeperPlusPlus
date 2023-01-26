#ifndef ITEMSDIALOG_H
#define ITEMSDIALOG_H

#include <QDialog>

namespace Ui {
class ItemsDialog;
}

class ItemsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ItemsDialog(int amountGoldenFlags, QWidget *parent = nullptr);
    ~ItemsDialog();

private:
    Ui::ItemsDialog *ui;

private slots:
    void useGoldenFlag();
};

#endif // ITEMSDIALOG_H
