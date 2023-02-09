#include "aboutdialog.h"
#include "ui_aboutdialog.h"

//constructor
aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);
}

//destructor
aboutDialog::~aboutDialog()
{
    delete ui;
}
