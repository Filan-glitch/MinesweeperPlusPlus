#include "gamechoicedialog.h"
#include "ui_gamechoicedialog.h"

GameChoiceDialog::GameChoiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameChoiceDialog)
{
    ui->setupUi(this);

    connect(ui->radio_easy, SIGNAL(clicked(bool)), this, SLOT(easyClicked(bool)));
    connect(ui->radio_intermediate, SIGNAL(clicked(bool)), this, SLOT(intermediateClicked(bool)));
    connect(ui->radio_hard, SIGNAL(clicked(bool)), this, SLOT(hardClicked(bool)));

}

GameChoiceDialog::~GameChoiceDialog()
{
    delete ui;
}

GameChoiceDialog::Choice GameChoiceDialog::getChoice() const
{
    return m_choice;
}


void GameChoiceDialog::easyClicked(bool checked)
{
    if(!checked) {
        ui->radio_easy->setChecked(true);
        return;
    }
    ui->radio_intermediate->setChecked(false);
    ui->radio_hard->setChecked(false);
    ui->buttonBox->setEnabled(true);
    m_choice = EASY;
}

void GameChoiceDialog::intermediateClicked(bool checked)
{
    if(!checked) {
        ui->radio_intermediate->setChecked(true);
        return;
    }
    ui->radio_easy->setChecked(false);
    ui->radio_hard->setChecked(false);
    ui->buttonBox->setEnabled(false);
    m_choice = INTERMEDIATE;
}

void GameChoiceDialog::hardClicked(bool checked)
{
    if(!checked) {
        ui->radio_hard->setChecked(true);
        return;
    }
    ui->radio_easy->setChecked(false);
    ui->radio_intermediate->setChecked(false);
    ui->buttonBox->setEnabled(false);
    m_choice = HARD;
}
