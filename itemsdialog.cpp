#include "itemsdialog.h"
#include "ui_itemsdialog.h"
#include "mainwindow.h"

ItemsDialog::ItemsDialog(int amountGoldenFlags, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemsDialog)
{
    ui->setupUi(this);
    ui->goldenFlagsLabel->setText(QString::asprintf("Golden Flags: %d", amountGoldenFlags));
    if(amountGoldenFlags == 0) ui->pushButtonUseGoldenFlag->setEnabled(false);
    connect(ui->pushButtonUseGoldenFlag, SIGNAL(clicked()), this, SLOT(useGoldenFlag()));
}

ItemsDialog::~ItemsDialog()
{
    delete ui;
}

void ItemsDialog::useGoldenFlag() {
    MainWindow* mainwindow = reinterpret_cast<MainWindow*>(this->parent());
    mainwindow->m_statsTracker->useGoldenFlag();
    mainwindow->useGoldenFlag();
    accept();
}
