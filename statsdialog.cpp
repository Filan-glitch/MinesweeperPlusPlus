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

    ui->confusionBestTimeLabel->setText("Best Time: " + QString::number(m_confusionStats["Best Time"].toInt()));
    ui->confusionTotalTimeLabel->setText("Total Time: " + QString::number(m_confusionStats["Total Time"].toInt()));
    ui->confusionWinsLabel->setText("Wins: " + QString::number(m_confusionStats["Wins"].toInt()));
    ui->confusionLosesLabel->setText("Loses: " + QString::number(m_confusionStats["Loses"].toInt()));
    ui->confusionEfficiencyLabel->setText("Efficiency: " + QString::number(m_confusionStats["Efficiency"].toDouble()));

    MainWindow* mainwindow = reinterpret_cast<MainWindow*>(this->parent());
    connect(ui->pushButtonReset, SIGNAL(clicked()), mainwindow->m_statsTracker, SLOT(resetStats()));
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

    ui->confusionBestTimeLabel->setText("Best Time: 0");
    ui->confusionTotalTimeLabel->setText("Total Time: 0");
    ui->confusionWinsLabel->setText("Wins: 0");
    ui->confusionLosesLabel->setText("Loses: 0");
    ui->confusionEfficiencyLabel->setText("Efficiency: 0");
    ui->pushButtonReset->setEnabled(false);
}

