#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include "gamechoicedialog.h"
#include "custompushbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer* m_timer;
    QTime* m_time;
    QList<CustomPushButton>* m_buttonList;
    QSet<int>* m_disabledButtonIDsList;
    QSet<int>* m_mineIDs;
    void startRound(GameChoiceDialog::Choice choice);
    void newEasyRound();



private slots:
    void updateTimer();
    void newGameChoice();
    void bombClicked();
    void reset();
    void disableClear(bool check);
    void about();
    void checkWin();


};
#endif // MAINWINDOW_H
