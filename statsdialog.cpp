#include "statsdialog.h"
#include "ui_statsdialog.h"
#include "mainwindow.h"

StatsDialog::StatsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatsDialog)
{
    ui->setupUi(this);
}

StatsDialog::~StatsDialog()
{
    delete ui;
}

StatsDialog::StatsDialog(const QJsonObject &easyStats, const QJsonObject &intermediateStats, const QJsonObject &hardStats, const QJsonObject &confusionStats, QWidget *parent) : QDialog(parent), ui(new Ui::StatsDialog),
    m_easyStats(easyStats),
    m_intermediateStats(intermediateStats),
    m_hardStats(hardStats),
    m_confusionStats(confusionStats)
{
    ui->setupUi(this);
    ui->easyBestTimeLabel->setText(QString::asprintf("Best Time: %s", m_easyStats["Best Time"].toString().toStdString().c_str()));
    ui->easyTotalTimeLabel->setText(QString::asprintf("Total Time: %s", m_easyStats["Total Time"].toString().toStdString().c_str()));
    ui->easyRoundsPlayedLabel->setText(QString::asprintf("Rounds Played: %s", m_easyStats["Rounds Played"].toString().toStdString().c_str()));
    ui->easyWinsLabel->setText(QString::asprintf("Wins: %s", m_easyStats["Wins"].toString().toStdString().c_str()));
    ui->easyLosesLabel->setText(QString::asprintf("Loses: %s", m_easyStats["Loses"].toString().toStdString().c_str()));
    ui->easyEfficiencyLabel->setText(QString::asprintf("Efficiency: %s", m_easyStats["Efficiency"].toString().toStdString().c_str()));

    ui->intermediateBestTimeLabel->setText(QString::asprintf("Best Time: %s", m_intermediateStats["Best Time"].toString().toStdString().c_str()));
    ui->intermediateTotalTimeLabel->setText(QString::asprintf("Total Time: %s", m_intermediateStats["Total Time"].toString().toStdString().c_str()));
    ui->intermediateRoundsPlayedLabel->setText(QString::asprintf("Rounds Played: %s", m_intermediateStats["Rounds Played"].toString().toStdString().c_str()));
    ui->intermediateWinsLabel->setText(QString::asprintf("Wins: %s", m_intermediateStats["Wins"].toString().toStdString().c_str()));
    ui->intermediateLosesLabel->setText(QString::asprintf("Loses: %s", m_intermediateStats["Loses"].toString().toStdString().c_str()));
    ui->intermediateEfficiencyLabel->setText(QString::asprintf("Efficiency: %s", m_intermediateStats["Efficiency"].toString().toStdString().c_str()));

    ui->hardBestTimeLabel->setText(QString::asprintf("Best Time: %s", m_hardStats["Best Time"].toString().toStdString().c_str()));
    ui->hardTotalTimeLabel->setText(QString::asprintf("Total Time: %s", m_hardStats["Total Time"].toString().toStdString().c_str()));
    ui->hardRoundsPlayedLabel->setText(QString::asprintf("Rounds Played: %s", m_hardStats["Rounds Played"].toString().toStdString().c_str()));
    ui->hardWinsLabel->setText(QString::asprintf("Wins: %s", m_hardStats["Wins"].toString().toStdString().c_str()));
    ui->hardLosesLabel->setText(QString::asprintf("Loses: %s", m_hardStats["Loses"].toString().toStdString().c_str()));
    ui->hardEfficiencyLabel->setText(QString::asprintf("Efficiency: %s", m_hardStats["Efficiency"].toString().toStdString().c_str()));

    ui->confusionBestTimeLabel->setText(QString::asprintf("Best Time: %s", m_confusionStats["Best Time"].toString().toStdString().c_str()));
    ui->confusionTotalTimeLabel->setText(QString::asprintf("Total Time: %s", m_confusionStats["Total Time"].toString().toStdString().c_str()));
    ui->confusionRoundsPlayedLabel->setText(QString::asprintf("Rounds Played: %s", m_confusionStats["Rounds Played"].toString().toStdString().c_str()));
    ui->confusionWinsLabel->setText(QString::asprintf("Wins: %s", m_confusionStats["Wins"].toString().toStdString().c_str()));
    ui->confusionLosesLabel->setText(QString::asprintf("Loses: %s", m_confusionStats["Loses"].toString().toStdString().c_str()));
    ui->confusionEfficiencyLabel->setText(QString::asprintf("Efficiency: %s", m_confusionStats["Efficiency"].toString().toStdString().c_str()));

    MainWindow* mainwindow = reinterpret_cast<MainWindow*>(this->parent());
    connect(ui->pushButtonReset, SIGNAL(clicked()), mainwindow->m_statsTracker, SLOT(resetStats()));
}
