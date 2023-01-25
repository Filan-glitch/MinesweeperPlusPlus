#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class StatsDialog;
}

class StatsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatsDialog(const QJsonObject &easyStats, const QJsonObject &intermediateStats, const QJsonObject &hardStats, const QJsonObject &confusionEasyStats, const QJsonObject &confusionIntermediateStats, const QJsonObject &confusionHardStats, QWidget *parent = nullptr);

    ~StatsDialog();

private slots:
    void on_pushButtonReset_clicked();

private:
    Ui::StatsDialog *ui;
    QJsonObject m_easyStats;
    QJsonObject m_intermediateStats;
    QJsonObject m_hardStats;
    QJsonObject m_confusionEasyStats;
    QJsonObject m_confusionIntermediateStats;
    QJsonObject m_confusionHardStats;
};

#endif // STATSDIALOG_H
