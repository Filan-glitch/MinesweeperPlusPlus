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
    explicit StatsDialog(QWidget *parent = nullptr);
    StatsDialog(const QJsonObject &easyStats, const QJsonObject &intermediateStats, const QJsonObject &hardStats, const QJsonObject &confusionStats, QWidget *parent = nullptr);
    ~StatsDialog();

private slots:
    void on_pushButtonReset_clicked();

private:
    Ui::StatsDialog *ui;
    QJsonObject m_easyStats;
    QJsonObject m_intermediateStats;
    QJsonObject m_hardStats;
    QJsonObject m_confusionStats;
};

#endif // STATSDIALOG_H
