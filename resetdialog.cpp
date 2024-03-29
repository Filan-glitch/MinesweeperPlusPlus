#include "resetdialog.h"
#include "ui_resetdialog.h"

//constructor
ResetDialog::ResetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResetDialog)
{
    ui->setupUi(this);
}

//destructor
ResetDialog::~ResetDialog()
{
    delete ui;
}

//getter if golden flags should be deleted
bool ResetDialog::resetGoldenFlag()
{
    return ui->goldenFlagCheckBox->isChecked();
}
