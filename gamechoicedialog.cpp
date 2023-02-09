#include "gamechoicedialog.h"
#include "ui_gamechoicedialog.h"

//constructor
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

//destructor
GameChoiceDialog::~GameChoiceDialog()
{
    delete ui;
}

//getter for the game choice
GameChoiceDialog::Choice GameChoiceDialog::getChoice() const
{
    return m_choice;
}

//setter to easy
void GameChoiceDialog::easyClicked(bool checked)
{
    m_choice = EASY;
}

//setter to intermediate
void GameChoiceDialog::intermediateClicked(bool checked)
{
    m_choice = INTERMEDIATE;
}

//setter to hard
void GameChoiceDialog::hardClicked(bool checked)
{
    m_choice = HARD;
}

//setter to confusion easy
void GameChoiceDialog::confusionEasyClicked(bool checked)
{
    m_choice = CONFUSION1;
}

//setter to confusion intermediate
void GameChoiceDialog::confusionIntermediateClicked(bool checked)
{
    m_choice = CONFUSION2;
}

//setter to confusion hard
void GameChoiceDialog::confusionHardClicked(bool checked)
{
    m_choice = CONFUSION3;
}

//setter to beginner easy
void GameChoiceDialog::beginnerEasyClicked(bool checked)
{
    m_choice = BEGINNER1;
}

//setter to beginner intermediate
void GameChoiceDialog::beginnerIntermediateClicked(bool checked)
{
    m_choice = BEGINNER2;
}
