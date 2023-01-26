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
    m_choice = EASY;
}

void GameChoiceDialog::intermediateClicked(bool checked)
{
    m_choice = INTERMEDIATE;
}

void GameChoiceDialog::hardClicked(bool checked)
{
    m_choice = HARD;
}

void GameChoiceDialog::confusionEasyClicked(bool checked)
{
    m_choice = CONFUSION1;
}

void GameChoiceDialog::confusionIntermediateClicked(bool checked)
{
    m_choice = CONFUSION2;
}

void GameChoiceDialog::confusionHardClicked(bool checked)
{
    m_choice = CONFUSION3;
}

void GameChoiceDialog::beginnerEasyClicked(bool checked)
{
    m_choice = BEGINNER1;
}

void GameChoiceDialog::beginnerIntermediateClicked(bool checked)
{
    m_choice = BEGINNER2;
}
