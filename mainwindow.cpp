#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "statsdialog.h"
#include "aboutdialog.h"
#include "itemsdialog.h"

//Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Setup
    ui->setupUi(this);

    //Initialisations
    m_mineIDs = new QVector<int>;
    m_buttonList = new QList<CustomPushButton*>;
    m_disabledButtonIDsList = new QSet<int>;
    m_markedButtonIDsList = new QSet<int>;
    m_statsTracker = new StatsTracker;

    //Timer
    m_time = new QTime(0,0,0);
    m_timer = new QTimer(this);
    ui->lcdTimer->display(m_time->toString("hh:mm:ss"));

    //Connections
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(reset()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionHow_to_play, SIGNAL(triggered()), this, SLOT(howToPlay()));
    connect(ui->actionShow_Result, SIGNAL(triggered()), this, SLOT(bombClicked()));
    connect(ui->actionStatistics, SIGNAL(triggered()), this, SLOT(showStats()));
    connect(ui->actionItems, SIGNAL(triggered()), this, SLOT(items()));
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
GameChoiceDialog::Choice MainWindow::currentMode() const
{
    return m_currentMode;
}

//Setter for current mode
void MainWindow::setCurrentMode(GameChoiceDialog::Choice newCurrentMode)
{
    m_currentMode = newCurrentMode;
}

//Function to start the round, enables the wanted widget and activates all needed modules
void MainWindow::startRound(GameChoiceDialog::Choice choice) {
    ui->centralwidget->setVisible(true);
    ui->playWidget->setVisible(true);
    ui->playWidget->setEnabled(true);
    ui->actionShow_Result->setEnabled(true);
    ui->actionItems->setEnabled(true);
    switch(choice) {
        case GameChoiceDialog::EASY: {
            m_currentMode = GameChoiceDialog::EASY;
            newEasyRound(false, false);
            break;
        }
        case GameChoiceDialog::INTERMEDIATE: {
            m_currentMode = GameChoiceDialog::INTERMEDIATE;
            newIntermediateRound(false, false);
            break;
        }
        case GameChoiceDialog::HARD: {
            m_currentMode = GameChoiceDialog::HARD;
            newHardRound(false);
            break;
        }
        case GameChoiceDialog::CONFUSION1: {
            m_currentMode = GameChoiceDialog::CONFUSION1;
            newEasyRound(true, false);
            break;
        }
        case GameChoiceDialog::CONFUSION2: {
            m_currentMode = GameChoiceDialog::CONFUSION2;
            newIntermediateRound(true, false);
            break;
        }
        case GameChoiceDialog::CONFUSION3: {
            m_currentMode = GameChoiceDialog::CONFUSION3;
            newHardRound(true);
            break;
        }
        case GameChoiceDialog::BEGINNER1: {
            m_currentMode = GameChoiceDialog::BEGINNER1;
            m_hearts = 2;
            newEasyRound(false, true);
            break;
        }
        case GameChoiceDialog::BEGINNER2: {
            m_currentMode = GameChoiceDialog::BEGINNER2;
            m_hearts = 3;
            newIntermediateRound(false, true);
            break;
        }
    }

}

//function that gets accessed, when the timer fires to update the timer LED
void MainWindow::updateTimer() {
   QTime tempTime = m_time->addSecs(1);
   m_currentRoundPlaytime += 1;
   m_time->setHMS(tempTime.hour(),tempTime.minute(),tempTime.second(),tempTime.msec());
   ui->lcdTimer->display(m_time->toString("hh:mm:ss"));
}

//function that setups a new easy round
void MainWindow::newEasyRound(bool confusion, bool beginner) {
    //Widget Size
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.67;
    screenSize.setWidth(screenSize.height());
    screenSize.setHeight(screenSize.height() * 1.2);
    setFixedSize(screenSize);

    screenSize.setHeight(screenSize.width() * 0.987);
    screenSize.setWidth(screenSize.width() * 0.987);
    ui->playWidget->setFixedSize(screenSize);
    ui->heartsLabel->setFixedSize(screenSize.width() * 0.4, screenSize.height() * 0.08);
    if(!beginner) {
        changeHearts(1, &screenSize);
    } else {
        changeHearts(2, &screenSize);
    }

    //Button List
    //rows
    constexpr std::size_t rows = 9;
    constexpr std::size_t columns = 9;
    constexpr std::size_t buttonCount = rows * columns;
    m_buttonList->reserve(buttonCount);
    const auto buttonSize = screenSize / rows;
    for(size_t row = 0; row < rows; ++row) {
        //columns
        for(size_t column = 0; column < columns; ++column){
        auto button = new CustomPushButton(ui->playWidget);
        button->setCustomIcon(CustomPushButton::CLEAR);
        button->setGeometry(0, 0, buttonSize.height(), buttonSize.width());
        reinterpret_cast<QGridLayout*>(ui->playWidget->layout())->addWidget(button, row, column);
        button->setFixedSize(buttonSize);
        button->setMinimumSize(buttonSize);
        button->setMaximumSize(buttonSize);
        button->setIconSize(buttonSize);
        m_buttonList->append(button);
        }
    }
    //Add Neighbours to every button and connection
    for(int i = 0; i < m_buttonList->size(); i++) {
        QVector<int> neighboursOffset = {-10,-9,-8,-1,1,8,9,10};
        if(i < columns) {
            neighboursOffset.removeOne(-10);
            neighboursOffset.removeOne(-9);
            neighboursOffset.removeOne(-8);
        } else if(i >= buttonCount - columns) {
            neighboursOffset.removeOne(10);
            neighboursOffset.removeOne(9);
            neighboursOffset.removeOne(8);
        }
        if(i % columns == 0) {
            neighboursOffset.removeOne(-10);
            neighboursOffset.removeOne(-1);
            neighboursOffset.removeOne(8);
        } else if((i + 1) % columns == 0) {
            neighboursOffset.removeOne(10);
            neighboursOffset.removeOne(1);
            neighboursOffset.removeOne(-8);
        }
        for(const auto j : neighboursOffset) {
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
        connect(button, SIGNAL(start()), this, SLOT(start()));
        connect(button, SIGNAL(clicked()), this, SLOT(addClick()));
    }

    //Random Bomb Generation
    generateBombs(9, 9, 10);
    for(const auto mineID : qAsConst(*m_mineIDs)) {
        (*m_buttonList)[mineID]->setCustomIcon(CustomPushButton::BOMB);
        (*m_buttonList)[mineID]->setIsMine(true);
        connect(m_buttonList->at(mineID), SIGNAL(clicked()), this, SLOT(bombClicked()));
    }
    for(auto i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        button->evaluateNeighbours();
        if(confusion) button->confuse();
        if(!button->isMine()) {
            connect(button, SIGNAL(clicked()), this, SLOT(disableClear()));
        }
    }
    calculate3BV();
    ui->lcdNumber->display(10);
}

//function that setups a new intermediate round
void MainWindow::newIntermediateRound(bool confusion, bool beginner) {
    //Widget Size
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.67;
    screenSize.setWidth(screenSize.height());
    screenSize.setHeight(screenSize.height() * 1.2);
    setFixedSize(screenSize);

    screenSize.setHeight(screenSize.width() * 0.987);
    screenSize.setWidth(screenSize.width() * 0.987);
    ui->playWidget->setFixedSize(screenSize);
    ui->heartsLabel->setFixedSize(screenSize.width() * 0.4, screenSize.height() * 0.08);
    if(!beginner) {
        changeHearts(1, &screenSize);
    } else {
        changeHearts(3, &screenSize);
    }

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
        connect(button, SIGNAL(start()), this, SLOT(start()));
        connect(button, SIGNAL(clicked()), this, SLOT(addClick()));
    }

    //Random Bomb Generation
    generateBombs(16, 16, 40);

    for(const auto mineID : qAsConst(*m_mineIDs)) {
        (*m_buttonList)[mineID]->setCustomIcon(CustomPushButton::BOMB);
        (*m_buttonList)[mineID]->setIsMine(true);
        connect(m_buttonList->at(mineID), SIGNAL(clicked()), this, SLOT(bombClicked()));
    }
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        button->evaluateNeighbours();
        if(confusion) button->confuse();
        if(!button->isMine()) {
            connect(button, SIGNAL(clicked()), this, SLOT(disableClear()));
        }
    }
    calculate3BV();
    ui->lcdNumber->display(40);
}

//function that setups a new hard round
void MainWindow::newHardRound(bool confusion) {

    //Widget Size
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.67;
    screenSize.setWidth(screenSize.height() * 1.875);
    screenSize.setHeight(screenSize.height() * 1.25);
    setFixedSize(screenSize);


    screenSize.setHeight((screenSize.width() / 1.875) * 0.987);
    screenSize.setWidth(screenSize.width() * 0.987);
    ui->playWidget->setFixedSize(screenSize);
    ui->heartsLabel->setFixedSize(screenSize.width() * 0.4, screenSize.height() * 0.16);
    changeHearts(1, &screenSize);

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
        connect(button, SIGNAL(start()), this, SLOT(start()));
        connect(button, SIGNAL(clicked()), this, SLOT(addClick()));
    }

    //Random Bomb Generation
    generateBombs(16, 30, 99);

    for(const auto mineID : qAsConst(*m_mineIDs)) {
        (*m_buttonList)[mineID]->setCustomIcon(CustomPushButton::BOMB);
        (*m_buttonList)[mineID]->setIsMine(true);
        connect(m_buttonList->at(mineID), SIGNAL(clicked()), this, SLOT(bombClicked()));
    }
    for(auto i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        button->evaluateNeighbours();
        if(confusion) button->confuse();
        if(!button->isMine()) {
            connect(button, SIGNAL(clicked()), this, SLOT(disableClear()));
        }
    }
    calculate3BV();
    ui->lcdNumber->display(99);

}

//function that calculates the minimum required clicks to win
void MainWindow::calculate3BV()
{
    int sizeMarkedIDs = 0;
    //calculating the clear buttons 3bv
    do {
        sizeMarkedIDs = m_markedButtonIDsList->size();
        calculateClear3BV();
    } while(sizeMarkedIDs < m_markedButtonIDsList->size());

    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        if(!button->isMarked() && !button->isMine()) {
            button->setIsMarked(true);
            m_3bv++;
        }
    }
}

//function that calculates the 3bv for the clear buttons and their neighbours
void MainWindow::calculateClear3BV() {
    //calculating the clear buttons 3bv
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        if(!button->isMarked() &&
            button->role() == CustomPushButton::CLEAR &&
            !m_markedButtonIDsList->contains(i)) {
            button->setIsMarked(true);
            button->markNeighbours();
            m_3bv++;
            break;
        }
    }

    //Adding all new marked buttons
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        if(button->isMarked()) {
            m_markedButtonIDsList->insert(i);
        }
    }
}

//function that calculates the position of the bombs and stores it in the m_minesIDs vector
void MainWindow::generateBombs(int rows, int columns, int bombs)
{
    std::vector<std::pair<int, int>> positions;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            positions.push_back(std::make_pair(i, j));
        }
    }
    std::random_shuffle(positions.begin(), positions.end());

    for(int i = 0; i < bombs; i++) {
        m_mineIDs->push_back(positions[i].first * columns + positions[i].second);
    }
}

//function that changes the visuals of the hearts label
void MainWindow::changeHearts(int amount, QSize* screenSize)
{
    QPixmap pixmap0(":/ressources/0lives.png");
    QPixmap pixmap1(":/ressources/1lives.png");
    QPixmap pixmap2(":/ressources/2lives.png");
    QPixmap pixmap3(":/ressources/3lives.png");
    pixmap0.scaled(screenSize->width() * 0.4, screenSize->height() * 0.16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap1.scaled(screenSize->width() * 0.4, screenSize->height() * 0.16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap2.scaled(screenSize->width() * 0.4, screenSize->height() * 0.16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap3.scaled(screenSize->width() * 0.4, screenSize->height() * 0.16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    switch(amount) {
    case 0:
        ui->heartsLabel->setPixmap(pixmap0);
        break;
    case 1:
        ui->heartsLabel->setPixmap(pixmap1);
        break;
    case 2:
        ui->heartsLabel->setPixmap(pixmap2);
        break;
    case 3:
        ui->heartsLabel->setPixmap(pixmap3);
        break;
    }
}

//function that changes the visuals of the hearts label
void MainWindow::changeHearts(int amount)
{
    QPixmap pixmap0(":/ressources/0lives.png");
    QPixmap pixmap1(":/ressources/1lives.png");
    QPixmap pixmap2(":/ressources/2lives.png");
    QPixmap pixmap3(":/ressources/3lives.png");
    pixmap0.scaled(ui->heartsLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap1.scaled(ui->heartsLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap2.scaled(ui->heartsLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap3.scaled(ui->heartsLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    switch(amount) {
    case 0:
        ui->heartsLabel->setPixmap(pixmap0);
        break;
    case 1:
        ui->heartsLabel->setPixmap(pixmap1);
        break;
    case 2:
        ui->heartsLabel->setPixmap(pixmap2);
        break;
    case 3:
        ui->heartsLabel->setPixmap(pixmap3);
        break;
    }
}

//function that updates the stats.json file
void MainWindow::updateStats()
{
    m_statsTracker->writeStats();
}

//function that executes the stats dialog
void MainWindow::showStats()
{
    bool wasActive = m_timer->isActive();
    if(wasActive) m_timer->stop();
    StatsDialog dlg(m_statsTracker->easyStats(), m_statsTracker->intermediateStats(), m_statsTracker->hardStats(), m_statsTracker->confusionEasyStats(), m_statsTracker->confusionIntermediateStats(), m_statsTracker->confusionHardStats(), this);
    dlg.exec();
    if(wasActive) m_timer->start(1000);
}

//function that gets accessed, when a mine gets clicked
void MainWindow::bombClicked() {
    if(m_hearts == 1) {
        m_statsTracker->roundsPlayedUpdate(m_currentMode, false, m_currentRoundPlaytime);
        for(int i = 0; i < m_buttonList->size(); i++) {
            CustomPushButton* button = (*m_buttonList)[i];
            if(!button->isMine() && button->icon() == CustomPushButton::FLAG) button->setCustomIcon(button->role());
        }
        ui->playWidget->setEnabled(false);
        m_timer->stop();
        ui->actionShow_Result->setEnabled(false);
        ui->actionItems->setEnabled(false);
        ui->lcdNumber->display(0);
        m_roundEnded = true;
        if(!m_started) obtainGoldenFlag();
    } else {
        m_bombsClicked++;
        ui->lcdNumber->display(ui->lcdNumber->intValue() - 1);
    }
    m_hearts -= 1;
    changeHearts(m_hearts);
}

//funtion that gets accessed, when the reset button is clicked
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
    m_disabledButtonIDsList->clear();
    m_markedButtonIDsList->clear();
    m_currentRoundPlaytime = 0;
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.67;
    screenSize.setWidth(screenSize.height());
    screenSize.setHeight(screenSize.height());
    setFixedSize(screenSize);
    ui->menubar->setVisible(false);
    m_menuImage->setVisible(true);
    ui->heartsLabel->clear();
    m_hearts = 1;
    m_started = false;
    m_roundEnded = false;
    m_3bv = 0;
    m_clicks = 0;
    m_goldenFlagsUsed = 0;
    m_bombsClicked = 0;
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
    bool wasActive = m_timer->isActive();
    if(wasActive) m_timer->stop();
    aboutDialog dlg(this);
    dlg.exec();
    if(wasActive) m_timer->start(1000);
}

//function that executes the items dialog
void MainWindow::items()
{
    bool wasActive = m_timer->isActive();
    if(wasActive) m_timer->stop();
    ItemsDialog dlg(m_statsTracker->amountGoldenFlags()["Golden Flags"].toInt(), this);
    dlg.exec();
    if(wasActive) m_timer->start(1000);
}

//function that checks, if the round is correctly won
void MainWindow::checkWin() {
    if ((m_currentMode == GameChoiceDialog::EASY && m_disabledButtonIDsList->size() == (71 + m_goldenFlagsUsed)) ||
        (m_currentMode == GameChoiceDialog::CONFUSION1 && m_disabledButtonIDsList->size() == (71 + m_goldenFlagsUsed)) ||
        (m_currentMode == GameChoiceDialog::BEGINNER1 && m_disabledButtonIDsList->size() == (71 + m_goldenFlagsUsed + m_bombsClicked)) ||
            (m_currentMode == GameChoiceDialog::INTERMEDIATE && m_disabledButtonIDsList->size() == (216 + m_goldenFlagsUsed)) ||
            (m_currentMode == GameChoiceDialog::CONFUSION2 && m_disabledButtonIDsList->size() == (216 + m_goldenFlagsUsed)) ||
            (m_currentMode == GameChoiceDialog::BEGINNER2 && m_disabledButtonIDsList->size() == (216 + m_goldenFlagsUsed + m_bombsClicked)) ||
            (m_currentMode == GameChoiceDialog::HARD && m_disabledButtonIDsList->size() == (381 + m_goldenFlagsUsed)) ||
            (m_currentMode == GameChoiceDialog::CONFUSION3 && m_disabledButtonIDsList->size() == (381 + m_goldenFlagsUsed))) {
        //setting all left mines to flags
        for(const auto mineID : qAsConst(*m_mineIDs)) {
            (*m_buttonList)[mineID]->setCustomIcon(CustomPushButton::FLAG);
        }
        ui->playWidget->setEnabled(false);
        ui->actionItems->setEnabled(false);
        ui->actionShow_Result->setEnabled(false);
        m_timer->stop();

        QMessageBox dlg(this);
        dlg.setWindowTitle("Win Screen");
        dlg.setWindowIcon(QIcon(":/ressources/minesweeper_logo.png"));
        dlg.setIcon(QMessageBox::Information);

        if(m_goldenFlagsUsed == 0) {
            double efficiency = calculateEfficiency();
            dlg.setText("You have won!\n\n3BV was: " + QString::number(m_3bv) + "\nYou needed: " + QString::number(m_clicks) + "\nEfficiency: " + QString::asprintf("%.2lf\%", efficiency * 100));
            m_statsTracker->roundsPlayedUpdate(m_currentMode, true, m_currentRoundPlaytime, efficiency);
        } else {
            dlg.setText("You have won!\n\n3BV was: " + QString::number(m_3bv) + "\nEfficiency not calculated, because you used a golden flag.");
            m_statsTracker->roundsPlayedUpdate(m_currentMode, true, m_currentRoundPlaytime);
        }
        dlg.exec();
        m_roundEnded = true;
    }
}

//function that reacts to Start Menu clicked
void MainWindow::startMenu() {
    GameChoiceDialog dlg(this);
    dlg.setBaseSize(this->size()*0.33);
    if(dlg.exec() == QDialog::Accepted) {
        ui->menubar->setVisible(true);
        m_menuImage->setVisible(false);
        startRound(dlg.getChoice());
    }
}

//function that starts a round
void MainWindow::start() {
    if(!m_started && !m_roundEnded) {
        m_timer->start(1000);
        m_started = true;
    }
}

//function that gives golden flag
void MainWindow::obtainGoldenFlag() {
    m_statsTracker->addGoldenFlag();

    QMessageBox dlg;
    dlg.setIcon(QMessageBox::Information);
    dlg.setInformativeText("You gained a golden flag. If used, it can reveal a bomb and its surroundings instantly.");
    dlg.setWindowIcon(QIcon(":ressources/minesweeper_logo.png"));
    dlg.setWindowTitle("Secret Item Gained!");
    dlg.exec();
}

//function that uses a golden flag
void MainWindow::useGoldenFlag() {
    default_random_engine generator(rand());
    uniform_int_distribution<int> possibleIDs(0, m_mineIDs->size()-1);
    int id;
    do {
    id = possibleIDs(generator);
    } while (((*m_buttonList)[m_mineIDs->at(id)])->icon() == CustomPushButton::FLAG);

    ((*m_buttonList)[m_mineIDs->at(id)])->setCustomIcon(CustomPushButton::GOLD);
    ((*m_buttonList)[m_mineIDs->at(id)])->setEnabled(false);
    ((*m_buttonList)[m_mineIDs->at(id)])->disableNeighbours();
    ui->lcdNumber->display(ui->lcdNumber->intValue() - 1);
    m_goldenFlagsUsed++;

    //Adding all new disabled buttons
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        if(!button->isEnabled()) {
            m_disabledButtonIDsList->insert(i);
        }
    }
}

//function that adds a click
void MainWindow::addClick() {
    m_clicks++;
}

//function that calculates the efficiency
double MainWindow::calculateEfficiency() {
    switch(m_currentMode) {
    case GameChoiceDialog::EASY:
    case GameChoiceDialog::CONFUSION1:
    case GameChoiceDialog::BEGINNER1: {
        int maxAmountOfClickDifference = 71 - m_3bv;
        double efficiency = static_cast<double>((71 - m_clicks)) / maxAmountOfClickDifference;
        return efficiency;
    }
    case GameChoiceDialog::INTERMEDIATE:
    case GameChoiceDialog::CONFUSION2:
    case GameChoiceDialog::BEGINNER2: {
        int maxAmountOfClickDifference = 216 - m_3bv;
        double efficiency = static_cast<double>((216 - m_clicks)) / maxAmountOfClickDifference;
        return efficiency;
    }
    case GameChoiceDialog::HARD:
    case GameChoiceDialog::CONFUSION3: {
        int maxAmountOfClickDifference = 381 - m_3bv;
        double efficiency = static_cast<double>((381 - m_clicks)) / maxAmountOfClickDifference;
        return efficiency;
    }
    default:
        return 0;
    }
}

//function that opens a website with minesweeper instructions
void MainWindow::howToPlay() {
    QDesktopServices::openUrl(QUrl("https://minesweepergame.com/strategy/how-to-play-minesweeper.php"));
}
