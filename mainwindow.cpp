#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "statsdialog.h"
#include "aboutdialog.h"


//Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Setup
    ui->setupUi(this);

    //Initialisations
    m_mineIDs = new QSet<int>;
    m_buttonList = new QList<CustomPushButton*>;
    m_disabledButtonIDsList = new QSet<int>;
    m_statsTracker = new StatsTracker;

    //Timer
    m_time = new QTime(0,0,0);
    m_timer = new QTimer(this);
    ui->lcdTimer->display(m_time->toString("hh:mm:ss"));

    //Connections
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(reset()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionShow_Result, SIGNAL(triggered()), this, SLOT(bombClicked()));
    connect(ui->actionStatistics, SIGNAL(triggered()), this, SLOT(showStats()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(checkWin()));

    //Widget
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.67;
    screenSize.setWidth(screenSize.height());
    screenSize.setHeight(screenSize.height());
    setFixedSize(screenSize);
    screenSize.setHeight(screenSize.width() * 0.987);
    screenSize.setWidth(screenSize.width() * 0.987);
    ui->playWidget->setFixedSize(screenSize);
    QLayout* layout = new QGridLayout(this);
    ui->playWidget->setLayout(layout);
    this->layout()->setSpacing(0);
    screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.67;
    screenSize.setWidth(screenSize.height());
    screenSize.setHeight(screenSize.height());

    //Start Menu
    m_menuImage = new QPushButton(this);
    m_menuImage->setGeometry(0,0,screenSize.width(), screenSize.height());
    QPixmap image(":/ressources/start_menu.png");
    image = image.scaled(screenSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_menuImage->setIcon(image);
    m_menuImage->setIconSize(screenSize);
    QLayout* generalLayout = new QVBoxLayout(this);
    generalLayout->addWidget(m_menuImage);
    this->setLayout(generalLayout);
    connect(m_menuImage, SIGNAL(clicked()), this, SLOT(startMenu()));

    //Initial State
    ui->centralwidget->setVisible(false);
    ui->actionReset->setVisible(false);
    ui->actionShow_Result->setEnabled(false);
    ui->menubar->setVisible(false);

}

//Destructor
MainWindow::~MainWindow()
{
    m_statsTracker->writeStats();
    delete ui;
}

//Getter for current mode
GameChoiceDialog::Choice MainWindow::currectMode() const
{
    return m_currentMode;
}

//Setter for current mode
void MainWindow::setCurrectMode(GameChoiceDialog::Choice newCurrectMode)
{
    m_currentMode = newCurrectMode;
}

//Function to start the round, enables the wanted widget and activates all needed modules
void MainWindow::startRound(GameChoiceDialog::Choice choice) {
    m_timer->start(1000);
    ui->centralwidget->setVisible(true);
    ui->playWidget->setVisible(true);
    ui->playWidget->setEnabled(true);
    ui->actionReset->setVisible(true);
    ui->actionShow_Result->setEnabled(true);
    switch(choice) {
        case GameChoiceDialog::EASY: {
            m_currentMode = GameChoiceDialog::EASY;
            newEasyRound();
            break;
        }
        case GameChoiceDialog::INTERMEDIATE: {
            m_currentMode = GameChoiceDialog::INTERMEDIATE;
            newIntermediateRound();
            break;
        }
        case GameChoiceDialog::HARD: {
            m_currentMode = GameChoiceDialog::HARD;
            newHardRound();
            break;
        }
        case GameChoiceDialog::CONFUSION: {
            m_currentMode = GameChoiceDialog::CONFUSION;
            newConfusionRound();
            break;
        }
        case GameChoiceDialog::BEGINNER1: {
            m_currentMode = GameChoiceDialog::BEGINNER1;
            break;
        }
        case GameChoiceDialog::BEGINNER2: {
            m_currentMode = GameChoiceDialog::BEGINNER2;
            break;
        }
    }

}

//slot that gets accessed, when the timer fires to update the timer LED
void MainWindow::updateTimer() {
   QTime tempTime = m_time->addSecs(1);
   m_currentRoundPlaytime += 1;
   m_time->setHMS(tempTime.hour(),tempTime.minute(),tempTime.second(),tempTime.msec());
   ui->lcdTimer->display(m_time->toString("hh:mm:ss"));
}

//function that setups a new easy round
void MainWindow::newEasyRound() {
    //Widget Size
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.67;
    screenSize.setWidth(screenSize.height());
    screenSize.setHeight(screenSize.height() * 1.2);
    setFixedSize(screenSize);

    screenSize.setHeight(screenSize.width() * 0.987);
    screenSize.setWidth(screenSize.width() * 0.987);
    ui->playWidget->setFixedSize(screenSize);
    ui->horizontalSpacer->changeSize(screenSize.width() * 0.5, screenSize.height() * 0.05, QSizePolicy::Fixed);

    //Button List
    //rows
    for(int row = 0; row < 9 ; row++) {
        //columns
        for(int column = 0; column < 9; column ++){
        CustomPushButton* button = new CustomPushButton(ui->playWidget);
        button->setCustomIcon(CustomPushButton::CLEAR);
        button->setGeometry(0, 0, ui->playWidget->size().height() / 9, ui->playWidget->size().width() / 9);
        reinterpret_cast<QGridLayout*>(ui->playWidget->layout())->addWidget(button, row, column);
        button->setFixedSize(ui->playWidget->size() / 9);
        button->setMinimumSize(ui->playWidget->size() / 9);
        button->setMaximumSize(ui->playWidget->size() / 9);
        button->setIconSize(ui->playWidget->size() / 9);
        m_buttonList->append(button);
        }
    }
    //Add Neighbours to every button and connection
    for(int i = 0; i < m_buttonList->size(); i++) {
        QVector<int> vec = {-10,-9,-8,-1,1,8,9,10};
        if(i < 9) {
            vec.removeOne(-10);
            vec.removeOne(-9);
            vec.removeOne(-8);
        } else if(i > 71) {
            vec.removeOne(10);
            vec.removeOne(9);
            vec.removeOne(8);
        }
        if((i % 9) == 0) {
            vec.removeOne(-10);
            vec.removeOne(-1);
            vec.removeOne(8);
        } else if(((i + 1) % 9) == 0) {
            vec.removeOne(10);
            vec.removeOne(1);
            vec.removeOne(-8);
        }
        for(int j : vec) {
            (*m_buttonList)[i]->addNeighbour((*m_buttonList)[i + j]);
        }
        CustomPushButton* button = (*m_buttonList)[i];
        connect(button, &QPushButton::customContextMenuRequested, [button]() {
                    MainWindow* window = reinterpret_cast<MainWindow*>(button->parent()->parent()->parent());
                    if(button->icon() == CustomPushButton::FLAG) {
                        button->setCustomIcon(button->role());
                        window->ui->lcdNumber->display(window->ui->lcdNumber->intValue() + 1);
                    } else if(window->ui->lcdNumber->intValue() > 0){
                       button->setCustomIcon(CustomPushButton::FLAG);
                       window->ui->lcdNumber->display(window->ui->lcdNumber->intValue() - 1);
                    }
                });
    }

    //Random Bomb Generation
    std::default_random_engine generator(rand());
    std::uniform_int_distribution<int> chance(0,80);
    while(m_mineIDs->size() != 10) {
        int mineID = chance(generator);
        //check for corners
        if(mineID == 0 || mineID == 8 || mineID == 72 || mineID == 80) continue;
        m_mineIDs->insert(mineID);
    }
    for(int mineID : *m_mineIDs) {
        (*m_buttonList)[mineID]->setCustomIcon(CustomPushButton::BOMB);
        (*m_buttonList)[mineID]->setIsMine(true);
        connect(m_buttonList->at(mineID), SIGNAL(clicked()), this, SLOT(bombClicked()));
    }
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        button->evaluateNeighbours();
        if(button->role() == CustomPushButton::CLEAR) {
            connect(button, SIGNAL(clicked()), this, SLOT(disableClear()));
        }
    }

    ui->lcdNumber->display(10);
}

//function that setups a new intermediate round
void MainWindow::newIntermediateRound() {
    //Widget Size
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.67;
    screenSize.setWidth(screenSize.height());
    screenSize.setHeight(screenSize.height() * 1.25);
    setFixedSize(screenSize);

    screenSize.setHeight(screenSize.width() * 0.987);
    screenSize.setWidth(screenSize.width() * 0.987);
    ui->playWidget->setFixedSize(screenSize);
    ui->horizontalSpacer->changeSize(screenSize.width() * 0.5, screenSize.height() * 0.05, QSizePolicy::Fixed);

    //Button List
    //rows
    for(int row = 0; row < 16 ; row++) {
        //columns
        for(int column = 0; column < 16; column ++){
        CustomPushButton* button = new CustomPushButton(ui->playWidget);
        button->setCustomIcon(CustomPushButton::CLEAR);
        button->setGeometry(0, 0, ui->playWidget->size().height() / 16, ui->playWidget->size().width() / 16);
        reinterpret_cast<QGridLayout*>(ui->playWidget->layout())->addWidget(button, row, column);
        button->setFixedSize(ui->playWidget->size() / 16);
        button->setMinimumSize(ui->playWidget->size() / 16);
        button->setMaximumSize(ui->playWidget->size() / 16);
        button->setIconSize(ui->playWidget->size() / 16);
        button->setObjectName(QString::asprintf("Row:%i | Column:%i", row, column));
        m_buttonList->append(button);
        }
    }
    //Add Neighbours to every button and connection
    for(int i = 0; i < m_buttonList->size(); i++) {
        QVector<int> vec = {-17,-16,-15,-1,1,15,16,17};
        if(i < 16) {
            vec.removeOne(-17);
            vec.removeOne(-16);
            vec.removeOne(-15);
        } else if(i > 239) {
            vec.removeOne(17);
            vec.removeOne(16);
            vec.removeOne(15);
        }
        if((i % 16) == 0) {
            vec.removeOne(-17);
            vec.removeOne(-1);
            vec.removeOne(15);
        } else if(((i + 1) % 16) == 0) {
            vec.removeOne(17);
            vec.removeOne(1);
            vec.removeOne(-15);
        }
        for(int j : vec) {
            (*m_buttonList)[i]->addNeighbour((*m_buttonList)[i + j]);
        }
        CustomPushButton* button = (*m_buttonList)[i];
        connect(button, &QPushButton::customContextMenuRequested, [button]() {
                    MainWindow* window = reinterpret_cast<MainWindow*>(button->parent()->parent()->parent());
                    if(button->icon() == CustomPushButton::FLAG) {
                        button->setCustomIcon(button->role());
                        window->ui->lcdNumber->display(window->ui->lcdNumber->intValue() + 1);
                    } else if(window->ui->lcdNumber->intValue() > 0){
                       button->setCustomIcon(CustomPushButton::FLAG);
                       window->ui->lcdNumber->display(window->ui->lcdNumber->intValue() - 1);
                    }
                });
    }

    //Random Bomb Generation
    std::default_random_engine generator(rand());
    std::uniform_int_distribution<int> chance(0,255);
    while(m_mineIDs->size() != 40) {
        int mineID = chance(generator);
        //check for corners
        if(mineID == 0 || mineID == 15 || mineID == 240 || mineID == 255) continue;
        m_mineIDs->insert(mineID);
    }
    for(int mineID : *m_mineIDs) {
        (*m_buttonList)[mineID]->setCustomIcon(CustomPushButton::BOMB);
        (*m_buttonList)[mineID]->setIsMine(true);
        connect(m_buttonList->at(mineID), SIGNAL(clicked()), this, SLOT(bombClicked()));
    }
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        button->evaluateNeighbours();
        if(button->role() == CustomPushButton::CLEAR) {
            connect(button, SIGNAL(clicked()), this, SLOT(disableClear()));
        }
    }

    ui->lcdNumber->display(40);

}

//function that setups a new hard round
void MainWindow::newHardRound() {

    //Widget Size
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.67;
    screenSize.setWidth(screenSize.height() * 1.875);
    screenSize.setHeight(screenSize.height() * 1.25);
    setFixedSize(screenSize);


    screenSize.setHeight((screenSize.width() / 1.875) * 0.987);
    screenSize.setWidth(screenSize.width() * 0.987);
    ui->playWidget->setFixedSize(screenSize);
    ui->horizontalSpacer->changeSize(screenSize.width() * 0.75, screenSize.height() * 0.05, QSizePolicy::Fixed);

    //Button List
    //rows
    for(int row = 0; row < 16 ; row++) {
        //columns
        for(int column = 0; column < 30; column ++){
        CustomPushButton* button = new CustomPushButton(ui->playWidget);
        button->setCustomIcon(CustomPushButton::CLEAR);
        QSize buttonSize(ui->playWidget->height() / 16, ui->playWidget->height() / 16);
        button->setGeometry(0, 0, buttonSize.height(), buttonSize.width());
        reinterpret_cast<QGridLayout*>(ui->playWidget->layout())->addWidget(button, row, column);
        button->setFixedSize(buttonSize);
        button->setMinimumSize(buttonSize);
        button->setMaximumSize(buttonSize);
        button->setIconSize(buttonSize);
        button->setObjectName(QString::asprintf("Row:%i | Column:%i", row, column));
        m_buttonList->append(button);
        }
    }
    //Add Neighbours to every button and connection
    for(int i = 0; i < m_buttonList->size(); i++) {
        QVector<int> vec = {-31,-30,-29,-1,1,29,30,31};
        if(i < 30) {
            vec.removeOne(-31);
            vec.removeOne(-30);
            vec.removeOne(-29);
        } else if(i > 449) {
            vec.removeOne(31);
            vec.removeOne(30);
            vec.removeOne(29);
        }
        if((i % 30) == 0) {
            vec.removeOne(-31);
            vec.removeOne(-1);
            vec.removeOne(29);
        } else if(((i + 1) % 30) == 0) {
            vec.removeOne(31);
            vec.removeOne(1);
            vec.removeOne(-29);
        }
        for(int j : vec) {
            (*m_buttonList)[i]->addNeighbour((*m_buttonList)[i + j]);
        }
        CustomPushButton* button = (*m_buttonList)[i];
        connect(button, &QPushButton::customContextMenuRequested, [button]() {
                    MainWindow* window = reinterpret_cast<MainWindow*>(button->parent()->parent()->parent());
                    if(button->icon() == CustomPushButton::FLAG) {
                        button->setCustomIcon(button->role());
                        window->ui->lcdNumber->display(window->ui->lcdNumber->intValue() + 1);
                    } else if(window->ui->lcdNumber->intValue() > 0){
                       button->setCustomIcon(CustomPushButton::FLAG);
                       window->ui->lcdNumber->display(window->ui->lcdNumber->intValue() - 1);
                    }
                });
    }

    //Random Bomb Generation
    std::default_random_engine generator(rand());
    std::uniform_int_distribution<int> chance(0,479);
    while(m_mineIDs->size() != 99) {
        int mineID = chance(generator);
        //check for corners
        if(mineID == 0 || mineID == 29 || mineID == 449 || mineID == 479) continue;
        m_mineIDs->insert(mineID);
    }
    for(int mineID : *m_mineIDs) {
        (*m_buttonList)[mineID]->setCustomIcon(CustomPushButton::BOMB);
        (*m_buttonList)[mineID]->setIsMine(true);
        connect(m_buttonList->at(mineID), SIGNAL(clicked()), this, SLOT(bombClicked()));
    }
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        button->evaluateNeighbours();
        if(button->role() == CustomPushButton::CLEAR) {
            connect(button, SIGNAL(clicked()), this, SLOT(disableClear()));
        }
    }

    ui->lcdNumber->display(99);

}

//function that setups a new confusion round
void MainWindow::newConfusionRound() {
    //Widget Size
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.67;
    screenSize.setWidth(screenSize.height());
    screenSize.setHeight(screenSize.height() * 1.2);
    setFixedSize(screenSize);

    screenSize.setHeight(screenSize.width() * 0.987);
    screenSize.setWidth(screenSize.width() * 0.987);
    ui->playWidget->setFixedSize(screenSize);
    ui->horizontalSpacer->changeSize(screenSize.width() * 0.5, screenSize.height() * 0.05, QSizePolicy::Fixed);

    //Button List
    //rows
    for(int row = 0; row < 9 ; row++) {
        //columns
        for(int column = 0; column < 9; column ++){
        CustomPushButton* button = new CustomPushButton(ui->playWidget);
        button->setCustomIcon(CustomPushButton::CLEAR);
        button->setGeometry(0, 0, ui->playWidget->size().height() / 9, ui->playWidget->size().width() / 9);
        reinterpret_cast<QGridLayout*>(ui->playWidget->layout())->addWidget(button, row, column);
        button->setFixedSize(ui->playWidget->size() / 9);
        button->setMinimumSize(ui->playWidget->size() / 9);
        button->setMaximumSize(ui->playWidget->size() / 9);
        button->setIconSize(ui->playWidget->size() / 9);
        m_buttonList->append(button);
        }
    }
    //Add Neighbours to every button and connection
    for(int i = 0; i < m_buttonList->size(); i++) {
        QVector<int> vec = {-10,-9,-8,-1,1,8,9,10};
        if(i < 9) {
            vec.removeOne(-10);
            vec.removeOne(-9);
            vec.removeOne(-8);
        } else if(i > 71) {
            vec.removeOne(10);
            vec.removeOne(9);
            vec.removeOne(8);
        }
        if((i % 9) == 0) {
            vec.removeOne(-10);
            vec.removeOne(-1);
            vec.removeOne(8);
        } else if(((i + 1) % 9) == 0) {
            vec.removeOne(10);
            vec.removeOne(1);
            vec.removeOne(-8);
        }
        for(int j : vec) {
            (*m_buttonList)[i]->addNeighbour((*m_buttonList)[i + j]);
        }
        CustomPushButton* button = (*m_buttonList)[i];
        connect(button, &QPushButton::customContextMenuRequested, [button]() {
                    MainWindow* window = reinterpret_cast<MainWindow*>(button->parent()->parent()->parent());
                    if(button->icon() == CustomPushButton::FLAG) {
                        button->setCustomIcon(button->role());
                        window->ui->lcdNumber->display(window->ui->lcdNumber->intValue() + 1);
                    } else if(window->ui->lcdNumber->intValue() > 0){
                       button->setCustomIcon(CustomPushButton::FLAG);
                       window->ui->lcdNumber->display(window->ui->lcdNumber->intValue() - 1);
                    }
                });
    }

    //Random Bomb Generation
    std::default_random_engine generator(rand());
    std::uniform_int_distribution<int> chance(0,80);
    while(m_mineIDs->size() != 10) {
        int mineID = chance(generator);
        //check for corners
        if(mineID == 0 || mineID == 8 || mineID == 72 || mineID == 80) continue;
        m_mineIDs->insert(mineID);
    }
    for(int mineID : *m_mineIDs) {
        (*m_buttonList)[mineID]->setCustomIcon(CustomPushButton::BOMB);
        (*m_buttonList)[mineID]->setIsMine(true);
        connect(m_buttonList->at(mineID), SIGNAL(clicked()), this, SLOT(bombClicked()));
    }
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        button->evaluateNeighbours();
        button->confuse();
        if(button->role() == CustomPushButton::CLEAR) {
            connect(button, SIGNAL(clicked()), this, SLOT(disableClear()));
        }
    }

    ui->lcdNumber->display(10);
}

//function that calculates the minimum required clicks to win
int MainWindow::calculateB3V()
{
    return 0;
}

//function that updates the stats.json file
void MainWindow::updateStats()
{
    m_statsTracker->writeStats();
}

//function that executes the stats dialog
void MainWindow::showStats()
{

    StatsDialog dlg(m_statsTracker->easyStats(), m_statsTracker->intermediateStats(), m_statsTracker->hardStats(), m_statsTracker->confusionStats(), this);
    dlg.exec();
}

//slot that gets accessed, when a mine gets clicked
void MainWindow::bombClicked() {
    m_statsTracker->roundsPlayedUpdate(m_currentMode, false, m_currentRoundPlaytime);
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        if(!button->isMine() && button->icon() == CustomPushButton::FLAG) button->setCustomIcon(button->role());
    }
    ui->playWidget->setEnabled(false);
    m_timer->stop();
    ui->actionReset->setVisible(true);
    ui->actionShow_Result->setEnabled(false);
}

//slot that gets accessed, when the reset button is clicked
void MainWindow::reset() {
    //Alle Buttons resetten
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        disconnect(button, nullptr, nullptr, nullptr);
        delete button;
    }
    //resetting ui and data
    ui->playWidget->setVisible(false);
    ui->centralwidget->setVisible(false);
    m_timer->stop();
    m_time->setHMS(0,0,0);
    m_buttonList->clear();
    m_mineIDs->clear();
    ui->lcdNumber->display(0);
    ui->lcdTimer->display("00:00");
    ui->actionShow_Result->setEnabled(false);
    ui->actionReset->setVisible(false);
    m_disabledButtonIDsList->clear();
    m_currentRoundPlaytime = 0;
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.67;
    screenSize.setWidth(screenSize.height());
    screenSize.setHeight(screenSize.height());
    setFixedSize(screenSize);
    ui->menubar->setVisible(false);
    m_menuImage->setVisible(true);
}

//function that floodfills all the clear buttons
void MainWindow::disableClear() {
    //Checking for the button that has been clicked and disabling its neighbours
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        if(!button->isEnabled() &&
            button->role() == CustomPushButton::CLEAR &&
            !m_disabledButtonIDsList->contains(i)) {
            button->disableNeighbours();
            break;
        }
    }
    //Adding all new disabled buttons
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        if(!button->isEnabled()) {
            m_disabledButtonIDsList->insert(i);
        }
    }
}

//function that executes the about dialog
void MainWindow::about() {
    aboutDialog dlg(this);
    dlg.exec();
}

//function that checks, if the round is correctly won
void MainWindow::checkWin() {
    int counter = 0;
    for(int mineID : *m_mineIDs) {
        CustomPushButton* button = (*m_buttonList)[mineID];
        if(button->icon() == CustomPushButton::FLAG) {
            counter++;
        }
    }
    if ((counter == 10 || counter == 40 || counter == 99) && ui->lcdNumber->value() == 0) {
        m_statsTracker->roundsPlayedUpdate(m_currentMode, true, m_currentRoundPlaytime);
        ui->playWidget->setEnabled(false);
        QMessageBox dlg(this);
        dlg.setWindowTitle("Win Screen");
        dlg.setWindowIcon(QIcon(":/ressources/minesweeper_logo.png"));
        dlg.setText("You have won!");
        dlg.setIcon(QMessageBox::Information);
        dlg.addButton("Reset", QMessageBox::AcceptRole);
        dlg.exec();
        reset();
    }
}

//function that reacts to Start Menu clicked
void MainWindow::startMenu() {
    GameChoiceDialog dlg(this);
    dlg.setFixedSize(this->size()*0.33);
    if(dlg.exec() == QDialog::Accepted) {
        ui->menubar->setVisible(true);
        m_menuImage->setVisible(false);
        startRound(dlg.getChoice());
    }

}
