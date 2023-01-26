#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>

namespace Ui {
class ResetDialog;
}

class ResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = nullptr);
    ~ResetDialog();
    bool resetGoldenFlag();

private:
    Ui::ResetDialog *ui;
};

#endif // RESETDIALOG_H
