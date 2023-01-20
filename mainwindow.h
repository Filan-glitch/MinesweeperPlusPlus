#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <random>
#include <QSet>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include "gamechoicedialog.h"
#include "custompushbutton.h"
#include "statstracker.h"
#include "gamechoicedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    GameChoiceDialog::Choice currectMode() const;
    void setCurrectMode(GameChoiceDialog::Choice newCurrectMode);
    StatsTracker* m_statsTracker;

private:
    Ui::MainWindow *ui;
    QTimer* m_timer;
    QTime* m_time;
    QList<CustomPushButton*>* m_buttonList;
    QSet<int>* m_disabledButtonIDsList;
    QSet<int>* m_mineIDs;

    GameChoiceDialog::Choice m_currentMode;
    int m_currentRoundPlaytime = 0;

    void startRound(GameChoiceDialog::Choice choice);
    void newEasyRound();
    void newIntermediateRound();
    void newHardRound();
    void newConfusionRound();
    int calculateB3V();

private slots:
    void updateTimer();
    void newGameChoice();
    void bombClicked();
    void reset();
    void disableClear();
    void about();
    void checkWin();
    void updateStats();
    void showStats();

};
#endif // MAINWINDOW_H
