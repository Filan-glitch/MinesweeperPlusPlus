#ifndef TROLLDIALOG_H
#define TROLLDIALOG_H

#include <QDialog>
#include <QMovie>

namespace Ui {
class TrollDialog;
}

class TrollDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrollDialog(QWidget *parent = nullptr);
    ~TrollDialog();

private:
    Ui::TrollDialog *ui;
};

#endif // TROLLDIALOG_H
