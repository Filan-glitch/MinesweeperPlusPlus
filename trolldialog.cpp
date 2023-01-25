#include "trolldialog.h"
#include "ui_trolldialog.h"

TrollDialog::TrollDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrollDialog)
{
    ui->setupUi(this);
    QMovie *movie = new QMovie(":/ressources/rickroll.gif");
    ui->trollLabel->setMovie(movie);
    movie->start();
}

TrollDialog::~TrollDialog()
{
    delete ui;
}
