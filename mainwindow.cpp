#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gamechoicedialog.h"
#include <QMessageBox>
#include <random>
#include <QSet>
#include "aboutdialog.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Setup
    ui->setupUi(this);
    setFixedSize(300,50);
    m_mineIDs = new QSet<int>;
    m_buttonList = new QList<CustomPushButton>;
    m_disabledButtonIDsList = new QSet<int>;
    ui->pushButton_reset->setVisible(false);
    ui->actionShow_Result->setEnabled(false);

    //Timer
    m_time = new QTime(0,0,0);
    m_timer = new QTimer(this);
    ui->lcdTimer->display(m_time->toString("hh:mm:ss"));


    //Connections
    connect(ui->actionNew_Game, SIGNAL(triggered()), this, SLOT(newGameChoice()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    connect(ui->pushButton_reset, SIGNAL(clicked()), this, SLOT(reset()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionShow_Result, SIGNAL(triggered()), this, SLOT(bombClicked()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(checkWin()));


    //Initial State
    ui->centralwidget->setVisible(false);
    ui->easyWidget->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Function to start the round, enables the wanted widget and activates all needed modules
void MainWindow::startRound(GameChoiceDialog::Choice choice) {
    m_timer->start(1000);
    ui->centralwidget->setVisible(true);
    switch(choice) {
    case GameChoiceDialog::EASY: {
        setFixedSize(628,739);
        ui->easyWidget->setVisible(true);
        ui->easyWidget->setEnabled(true);
        ui->actionNew_Game->setEnabled(false);
        ui->actionShow_Result->setEnabled(true);
        newEasyRound();
        break;
    }
    case GameChoiceDialog::INTERMEDIATE:
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
    //Button List
    for(int i = 1; i < ui->easyWidget->children().size(); i++) {
        m_buttonList->append(CustomPushButton(reinterpret_cast<QPushButton*>(ui->easyWidget->children().at(i))));
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
            (*m_buttonList)[i].addNeighbour(&(*m_buttonList)[i + j]);
        }
        CustomPushButton* button = &(*m_buttonList)[i];
        connect(button->button(), SIGNAL(clicked(bool)), (*m_buttonList)[i].button(), SLOT(setEnabled(bool)));
        connect(button->button(), &QPushButton::customContextMenuRequested,
                [button]() {
                    // Ändern Sie das Icon des QPushButton
                    if(button->icon() == CustomPushButton::FLAG) {
                        button->setIcon(button->role());
                    } else {
                       button->setIcon(CustomPushButton::FLAG);

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
        (*m_buttonList)[mineID].setIcon(CustomPushButton::BOMB);
        (*m_buttonList)[mineID].setIsMine(true);
        connect(m_buttonList->at(mineID).button(), SIGNAL(clicked()), this, SLOT(bombClicked()));
    }
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = &(*m_buttonList)[i];
        button->evaluateNeighbours();
        if(button->role() == CustomPushButton::CLEAR) {
            connect(button->button(), SIGNAL(clicked(bool)), this, SLOT(disableClear(bool)));
        }
    }

}

//slot that gets accessed, when a mine gets clicked
void MainWindow::bombClicked() {
    ui->easyWidget->setEnabled(false);
    m_timer->stop();
    ui->pushButton_reset->setVisible(true);
}

//slot that gets accessed, when the reset button is clicked
void MainWindow::reset() {
    //Mines resetten
    for(int mineID : *m_mineIDs) {
        (*m_buttonList)[mineID].setIsMine(false);
        disconnect(m_buttonList->at(mineID).button(), SIGNAL(clicked()), this, SLOT(bombClicked()));
    }
    //Alle Buttons resetten
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = &(*m_buttonList)[i];
        disconnect(button->button());
        button->setIcon(CustomPushButton::CLEAR);
        button->button()->setEnabled(true);
    }
    //UI resetten
    ui->easyWidget->setVisible(false);
    setFixedSize(300,50);
    ui->centralwidget->setVisible(false);
    m_timer->stop();
    m_time->setHMS(0,0,0);
    m_buttonList->clear();
    m_mineIDs->clear();
    ui->actionNew_Game->setEnabled(true);
    ui->actionShow_Result->setEnabled(false);
    ui->pushButton_reset->setVisible(false);
    m_disabledButtonIDsList->clear();
}

//Function that floodfills all the clear buttons
void MainWindow::disableClear(bool check) {
    //Checking for the button that has been clicked and disabling its neighbours
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = &(*m_buttonList)[i];
        if(!button->button()->isEnabled() &&
            button->role() == CustomPushButton::CLEAR &&
            !m_disabledButtonIDsList->contains(i)) {
            button->disableNeighbours();
            break;
        }
    }
    //Adding all new disabled buttons
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = &(*m_buttonList)[i];
        if(!button->button()->isEnabled()) {
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
    for(int i = 0; i < m_buttonList->size(); i++) {
        CustomPushButton* button = &(*m_buttonList)[i];
        if(button->button()->isEnabled()) {
            counter++;
        }
    }
    if (counter == 10) {
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
