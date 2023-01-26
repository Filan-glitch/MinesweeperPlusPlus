#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <random>
#include <QSet>
#include <QGuiApplication>
#include <QScreen>
#include "gamechoicedialog.h"
#include "custompushbutton.h"
#include "statstracker.h"
#include "gamechoicedialog.h"
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <algorithm>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    GameChoiceDialog::Choice currentMode() const;
    void setCurrentMode(GameChoiceDialog::Choice newCurrentMode);
    StatsTracker* m_statsTracker;
    void useGoldenFlag();

private:
    Ui::MainWindow *ui;
    QTimer* m_timer;
    QTime* m_time;
    QList<CustomPushButton*>* m_buttonList;
    QSet<int>* m_disabledButtonIDsList;
    QSet<int>* m_markedButtonIDsList;
    QVector<int>* m_mineIDs;
    QPushButton* m_menuImage;
    GameChoiceDialog::Choice m_currentMode;
    int m_currentRoundPlaytime = 0;
    int m_clicks = 0;
    int m_hearts = 1;
    bool m_started = false;
    bool m_roundEnded = false;
    int m_3bv = 0;
    int m_goldenFlagsUsed = 0;

    void startRound(GameChoiceDialog::Choice choice);
    void newEasyRound(bool confusion, bool beginner);
    void newIntermediateRound(bool confusion, bool beginner);
    void newHardRound(bool confusion);
    void calculate3BV();
    void calculateClear3BV();
    double calculateEfficiency();
    void generateBombs(int rows, int columns, int bombs);
    void changeHearts(int amount, QSize* screenSize);
    void changeHearts(int amount);
    void obtainGoldenFlag();

private slots:
    void updateTimer();
    void bombClicked();
    void reset();
    void disableClear();
    void about();
    void items();
    void checkWin();
    void updateStats();
    void showStats();
    void startMenu();
    void start();
    void addClick();
};
#endif // MAINWINDOW_H
