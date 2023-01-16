#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gamechoicedialog.h"
#include <QMessageBox>
#include <random>
#include <QSet>
#include "aboutdialog.h"
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Setup
    ui->setupUi(this);
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.5;
    screenSize.setWidth(screenSize.height() * 0.85);
    setFixedSize(screenSize);
    m_mineIDs = new QSet<int>;
    m_buttonList = new QList<CustomPushButton*>;
    m_disabledButtonIDsList = new QSet<int>;
    ui->actionReset->setVisible(false);
    ui->actionShow_Result->setEnabled(false);

    //Timer
    m_time = new QTime(0,0,0);
    m_timer = new QTimer(this);
    ui->lcdTimer->display(m_time->toString("hh:mm:ss"));

    //Connections
    connect(ui->actionNew_Game, SIGNAL(triggered()), this, SLOT(newGameChoice()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(reset()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionShow_Result, SIGNAL(triggered()), this, SLOT(bombClicked()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(checkWin()));


    //Initial State
    ui->centralwidget->setVisible(false);
    //Widget
    screenSize.setHeight(screenSize.width() * 0.987);
    screenSize.setWidth(screenSize.width() * 0.987);
    ui->playWidget->setFixedSize(screenSize);
    QLayout* layout = new QGridLayout(this);
    ui->playWidget->setLayout(layout);
    ui->horizontalSpacer->changeSize(screenSize.width() * 0.5, screenSize.height() * 0.05, QSizePolicy::Fixed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Function to start the round, enables the wanted widget and activates all needed modules
void MainWindow::startRound(GameChoiceDialog::Choice choice) {
    m_timer->start(1000);
    ui->centralwidget->setVisible(true);
    ui->playWidget->setVisible(true);
    ui->playWidget->setEnabled(true);
    ui->actionNew_Game->setVisible(false);
    ui->actionReset->setVisible(true);
    ui->actionShow_Result->setEnabled(true);
    switch(choice) {
    case GameChoiceDialog::EASY: {
        newEasyRound();
        break;
    }
    case GameChoiceDialog::INTERMEDIATE:
        newIntermediateRound();
        break;
    case GameChoiceDialog::HARD: {
        QMessageBox dlg;
        dlg.setIcon(QMessageBox::Warning);
        dlg.setText("Dieser Modus ist noch Work in Progress. Wähl einen anderen Modus aus!");
        dlg.addButton("OK", QMessageBox::AcceptRole);
        dlg.exec();
        break;
    }
    }

}

//slot that gets accessed, when the timer fires to update the timer LED
void MainWindow::updateTimer() {
   QTime tempTime = m_time->addSecs(1);
   m_time->setHMS(tempTime.hour(),tempTime.minute(),tempTime.second(),tempTime.msec());
   ui->lcdTimer->display(m_time->toString("hh:mm:ss"));
}

//slot that gets accessed, when the new game action triggeres
void MainWindow::newGameChoice() {
    GameChoiceDialog dlg;
    if(dlg.exec() == QDialog::Accepted) {
        startRound(dlg.getChoice());
    }
}

//function that setups a new easy round
void MainWindow::newEasyRound() {
    //Widget Size
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.5;
    screenSize.setWidth(screenSize.height() * 0.85);
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
                    qDebug() << "Button: " << button->objectName() << " icon: " << button->icon() << " role: " << button->role();
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
        m_mineIDs->insert(chance(generator));
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
    screenSize.setWidth(screenSize.height() * 0.85);
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
                    qDebug() << "Button: " << button->objectName() << " icon: " << button->icon() << " role: " << button->role();
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

//slot that gets accessed, when a mine gets clicked
void MainWindow::bombClicked() {
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = (*m_buttonList)[i];
        if(!button->isMine() && button->icon() == CustomPushButton::FLAG) button->setCustomIcon(button->role());
    }
    ui->playWidget->setEnabled(false);
    m_timer->stop();
    ui->actionNew_Game->setVisible(false);
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
    //UI resetten
    ui->playWidget->setVisible(false);
    ui->centralwidget->setVisible(false);
    m_timer->stop();
    m_time->setHMS(0,0,0);
    m_buttonList->clear();
    m_mineIDs->clear();
    ui->lcdNumber->display(0);
    ui->lcdTimer->display("00:00");
    ui->actionNew_Game->setVisible(true);
    ui->actionShow_Result->setEnabled(false);
    ui->actionReset->setVisible(false);
    m_disabledButtonIDsList->clear();

    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size() * 0.5;
    screenSize.setWidth(screenSize.height() * 0.85);
    setFixedSize(screenSize);

    screenSize.setHeight(screenSize.width() * 0.987);
    screenSize.setWidth(screenSize.width() * 0.987);
    ui->playWidget->setFixedSize(screenSize);
    ui->horizontalSpacer->changeSize(screenSize.width() * 0.5, screenSize.height() * 0.05, QSizePolicy::Fixed);
}

//Function that floodfills all the clear buttons
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

void MainWindow::about() {
    aboutDialog dlg;
    dlg.exec();
}

void MainWindow::checkWin() {
    int counter = 0;
    for(int mineID : *m_mineIDs) {
        CustomPushButton* button = (*m_buttonList)[mineID];
        if(button->icon() == CustomPushButton::FLAG) {
            counter++;
        }
    }
    if ((counter == 10 || counter == 40) && ui->lcdNumber->value() == 0) {
        ui->playWidget->setEnabled(false);
        QMessageBox dlg;
        dlg.setWindowTitle("Win Screen");
        dlg.setWindowIcon(QIcon(":/minesweeper_logo.png"));
        dlg.setText("You have won!");
        dlg.setIcon(QMessageBox::Information);
        dlg.addButton("Reset", QMessageBox::AcceptRole);
        dlg.exec();
        reset();
    }
}
