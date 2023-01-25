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
    connect(ui->radio_confusionEasy, SIGNAL(clicked(bool)), this, SLOT(confusionEasyClicked(bool)));
    connect(ui->radio_confusionIntermediate, SIGNAL(clicked(bool)), this, SLOT(confusionIntermediateClicked(bool)));
    connect(ui->radio_confusionHard, SIGNAL(clicked(bool)), this, SLOT(confusionHardClicked(bool)));
    connect(ui->radio_beginnerEasy, SIGNAL(clicked(bool)), this, SLOT(beginnerEasyClicked(bool)));
    connect(ui->radio_beginnerIntermediate, SIGNAL(clicked(bool)), this, SLOT(beginnerIntermediateClicked(bool)));

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
    ui->radio_confusionEasy->setChecked(false);
    ui->radio_confusionIntermediate->setChecked(false);
    ui->radio_confusionHard->setChecked(false);
    ui->radio_beginnerEasy->setChecked(false);
    ui->radio_beginnerIntermediate->setChecked(false);
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
    ui->radio_confusionEasy->setChecked(false);
    ui->radio_confusionIntermediate->setChecked(false);
    ui->radio_confusionHard->setChecked(false);
    ui->radio_beginnerEasy->setChecked(false);
    ui->radio_beginnerIntermediate->setChecked(false);
    ui->buttonBox->setEnabled(true);
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
    ui->radio_confusionEasy->setChecked(false);
    ui->radio_confusionIntermediate->setChecked(false);
    ui->radio_confusionHard->setChecked(false);
    ui->radio_beginnerEasy->setChecked(false);
    ui->radio_beginnerIntermediate->setChecked(false);
    ui->buttonBox->setEnabled(true);
    m_choice = HARD;
}

void GameChoiceDialog::confusionEasyClicked(bool checked)
{
    if(!checked) {
        ui->radio_confusionEasy->setChecked(true);
        return;
    }
    ui->radio_easy->setChecked(false);
    ui->radio_intermediate->setChecked(false);
    ui->radio_hard->setChecked(false);
    ui->radio_confusionIntermediate->setChecked(false);
    ui->radio_confusionHard->setChecked(false);
    ui->radio_beginnerEasy->setChecked(false);
    ui->radio_beginnerIntermediate->setChecked(false);
    ui->buttonBox->setEnabled(true);
    m_choice = CONFUSION1;
}

void GameChoiceDialog::confusionIntermediateClicked(bool checked)
{
    if(!checked) {
        ui->radio_confusionIntermediate->setChecked(true);
        return;
    }
    ui->radio_easy->setChecked(false);
    ui->radio_intermediate->setChecked(false);
    ui->radio_hard->setChecked(false);
    ui->radio_confusionEasy->setChecked(false);
    ui->radio_confusionHard->setChecked(false);
    ui->radio_beginnerEasy->setChecked(false);
    ui->radio_beginnerIntermediate->setChecked(false);
    ui->buttonBox->setEnabled(true);
    m_choice = CONFUSION2;
}

void GameChoiceDialog::confusionHardClicked(bool checked)
{
    if(!checked) {
        ui->radio_confusionHard->setChecked(true);
        return;
    }
    ui->radio_easy->setChecked(false);
    ui->radio_intermediate->setChecked(false);
    ui->radio_hard->setChecked(false);
    ui->radio_confusionEasy->setChecked(false);
    ui->radio_confusionIntermediate->setChecked(false);
    ui->radio_beginnerEasy->setChecked(false);
    ui->radio_beginnerIntermediate->setChecked(false);
    ui->buttonBox->setEnabled(true);
    m_choice = CONFUSION3;
}

void GameChoiceDialog::beginnerEasyClicked(bool checked)
{
    if(!checked) {
        ui->radio_beginnerEasy->setChecked(true);
        return;
    }
    ui->radio_easy->setChecked(false);
    ui->radio_intermediate->setChecked(false);
    ui->radio_hard->setChecked(false);
    ui->radio_confusionEasy->setChecked(false);
    ui->radio_confusionIntermediate->setChecked(false);
    ui->radio_confusionHard->setChecked(false);
    ui->radio_beginnerIntermediate->setChecked(false);
    ui->buttonBox->setEnabled(true);
    m_choice = BEGINNER1;
}

void GameChoiceDialog::beginnerIntermediateClicked(bool checked)
{
    if(!checked) {
        ui->radio_beginnerIntermediate->setChecked(true);
        return;
    }
    ui->radio_easy->setChecked(false);
    ui->radio_intermediate->setChecked(false);
    ui->radio_hard->setChecked(false);
    ui->radio_confusionEasy->setChecked(false);
    ui->radio_confusionIntermediate->setChecked(false);
    ui->radio_confusionHard->setChecked(false);
    ui->radio_beginnerEasy->setChecked(false);
    ui->buttonBox->setEnabled(true);
    m_choice = BEGINNER2;
}
