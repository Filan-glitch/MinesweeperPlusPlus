#include "statsdialog.h"
#include "ui_statsdialog.h"
#include "mainwindow.h"

StatsDialog::~StatsDialog()
{
    delete ui;
}

void StatsDialog::on_pushButtonReset_clicked()
{
    ui->easyBestTimeLabel->setText("Best Time: 0");
    ui->easyTotalTimeLabel->setText("Total Time: 0");
    ui->easyWinsLabel->setText("Wins: 0");
    ui->easyLosesLabel->setText("Loses: 0");
    ui->easyEfficiencyLabel->setText("Efficiency: 0");

    ui->intermediateBestTimeLabel->setText("Best Time: 0");
    ui->intermediateTotalTimeLabel->setText("Total Time: 0");
    ui->intermediateWinsLabel->setText("Wins: 0");
    ui->intermediateLosesLabel->setText("Loses: 0");
    ui->intermediateEfficiencyLabel->setText("Efficiency: 0");

    ui->hardBestTimeLabel->setText("Best Time: 0");
    ui->hardTotalTimeLabel->setText("Total Time: 0");
    ui->hardWinsLabel->setText("Wins: 0");
    ui->hardLosesLabel->setText("Loses: 0");
    ui->hardEfficiencyLabel->setText("Efficiency: 0");

    ui->confusionEasyBestTimeLabel->setText("Best Time: 0");
    ui->confusionEasyTotalTimeLabel->setText("Total Time: 0");
    ui->confusionEasyWinsLabel->setText("Wins: 0");
    ui->confusionEasyLosesLabel->setText("Loses: 0");
    ui->confusionEasyEfficiencyLabel->setText("Efficiency: 0");

    ui->confusionIntermediateBestTimeLabel->setText("Best Time: 0");
    ui->confusionIntermediateTotalTimeLabel->setText("Total Time: 0");
    ui->confusionIntermediateWinsLabel->setText("Wins: 0");
    ui->confusionIntermediateLosesLabel->setText("Loses: 0");
    ui->confusionIntermediateEfficiencyLabel->setText("Efficiency: 0");

    ui->confusionHardBestTimeLabel->setText("Best Time: 0");
    ui->confusionHardTotalTimeLabel->setText("Total Time: 0");
    ui->confusionHardWinsLabel->setText("Wins: 0");
    ui->confusionHardLosesLabel->setText("Loses: 0");
    ui->confusionHardEfficiencyLabel->setText("Efficiency: 0");

    ui->pushButtonReset->setEnabled(false);
}


StatsDialog::StatsDialog(const QJsonObject &easyStats, const QJsonObject &intermediateStats, const QJsonObject &hardStats, const QJsonObject &confusionEasyStats, const QJsonObject &confusionIntermediateStats, const QJsonObject &confusionHardStats, QWidget* parent) : QDialog(parent), ui(new Ui::StatsDialog),
    m_easyStats(easyStats),
    m_intermediateStats(intermediateStats),
    m_hardStats(hardStats),
    m_confusionEasyStats(confusionEasyStats),
    m_confusionIntermediateStats(confusionIntermediateStats),
    m_confusionHardStats(confusionHardStats)
{
    ui->setupUi(this);
    ui->easyBestTimeLabel->setText("Best Time: " + QString::number(m_easyStats["Best Time"].toInt()));
    ui->easyTotalTimeLabel->setText("Total Time: " + QString::number(m_easyStats["Total Time"].toInt()));
    ui->easyWinsLabel->setText("Wins: " + QString::number(m_easyStats["Wins"].toInt()));
    ui->easyLosesLabel->setText("Loses: " + QString::number(m_easyStats["Loses"].toInt()));
    ui->easyEfficiencyLabel->setText("Efficiency: " + QString::number(m_easyStats["Efficiency"].toDouble()));

    ui->intermediateBestTimeLabel->setText("Best Time: " + QString::number(m_intermediateStats["Best Time"].toInt()));
    ui->intermediateTotalTimeLabel->setText("Total Time: " + QString::number(m_intermediateStats["Total Time"].toInt()));
    ui->intermediateWinsLabel->setText("Wins: " + QString::number(m_intermediateStats["Wins"].toInt()));
    ui->intermediateLosesLabel->setText("Loses: " + QString::number(m_intermediateStats["Loses"].toInt()));
    ui->intermediateEfficiencyLabel->setText("Efficiency: " + QString::number(m_intermediateStats["Efficiency"].toDouble()));

    ui->hardBestTimeLabel->setText("Best Time: " + QString::number(m_hardStats["Best Time"].toInt()));
    ui->hardTotalTimeLabel->setText("Total Time: " + QString::number(m_hardStats["Total Time"].toInt()));
    ui->hardWinsLabel->setText("Wins: " + QString::number(m_hardStats["Wins"].toInt()));
    ui->hardLosesLabel->setText("Loses: " + QString::number(m_hardStats["Loses"].toInt()));
    ui->hardEfficiencyLabel->setText("Efficiency: " + QString::number(m_hardStats["Efficiency"].toDouble()));

    ui->confusionEasyBestTimeLabel->setText("Best Time: " + QString::number(m_confusionEasyStats["Best Time"].toInt()));
    ui->confusionEasyTotalTimeLabel->setText("Total Time: " + QString::number(m_confusionEasyStats["Total Time"].toInt()));
    ui->confusionEasyWinsLabel->setText("Wins: " + QString::number(m_confusionEasyStats["Wins"].toInt()));
    ui->confusionEasyLosesLabel->setText("Loses: " + QString::number(m_confusionEasyStats["Loses"].toInt()));
    ui->confusionEasyEfficiencyLabel->setText("Efficiency: " + QString::number(m_confusionEasyStats["Efficiency"].toDouble()));

    ui->confusionIntermediateBestTimeLabel->setText("Best Time: " + QString::number(m_confusionIntermediateStats["Best Time"].toInt()));
    ui->confusionIntermediateTotalTimeLabel->setText("Total Time: " + QString::number(m_confusionIntermediateStats["Total Time"].toInt()));
    ui->confusionIntermediateWinsLabel->setText("Wins: " + QString::number(m_confusionIntermediateStats["Wins"].toInt()));
    ui->confusionIntermediateLosesLabel->setText("Loses: " + QString::number(m_confusionIntermediateStats["Loses"].toInt()));
    ui->confusionIntermediateEfficiencyLabel->setText("Efficiency: " + QString::number(m_confusionIntermediateStats["Efficiency"].toDouble()));

    ui->confusionHardBestTimeLabel->setText("Best Time: " + QString::number(m_confusionHardStats["Best Time"].toInt()));
    ui->confusionHardTotalTimeLabel->setText("Total Time: " + QString::number(m_confusionHardStats["Total Time"].toInt()));
    ui->confusionHardWinsLabel->setText("Wins: " + QString::number(m_confusionHardStats["Wins"].toInt()));
    ui->confusionHardLosesLabel->setText("Loses: " + QString::number(m_confusionHardStats["Loses"].toInt()));
    ui->confusionHardEfficiencyLabel->setText("Efficiency: " + QString::number(m_confusionHardStats["Efficiency"].toDouble()));

    MainWindow* mainwindow = reinterpret_cast<MainWindow*>(this->parent());
    connect(ui->pushButtonReset, SIGNAL(clicked()), mainwindow->m_statsTracker, SLOT(resetStats()));
}
