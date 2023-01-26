#include "resetdialog.h"
#include "ui_resetdialog.h"

ResetDialog::ResetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResetDialog)
{
    ui->setupUi(this);
}

ResetDialog::~ResetDialog()
{
    delete ui;
}

bool ResetDialog::resetGoldenFlag()
{
    return ui->goldenFlagCheckBox->isChecked();
}
