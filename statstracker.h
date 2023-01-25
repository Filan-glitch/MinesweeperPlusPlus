#ifndef STATSTRACKER_H
#define STATSTRACKER_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include "gamechoicedialog.h"

class StatsTracker : public QObject
{
    Q_OBJECT

public:
    StatsTracker();
    void readStats();
    void writeStats();

    QJsonObject easyStats() const;
    QJsonObject intermediateStats() const;
    QJsonObject hardStats() const;
    QJsonObject confusionEasyStats() const;
    QJsonObject confusionIntermediateStats() const;
    QJsonObject confusionHardStats() const;
    void roundsPlayedUpdate(GameChoiceDialog::Choice choice, bool win, int time, float efficiency = 0.0f);

private:
    QJsonObject m_easyStats;
    QJsonObject m_intermediateStats;
    QJsonObject m_hardStats;
    QJsonObject m_confusionEasyStats;
    QJsonObject m_confusionIntermediateStats;
    QJsonObject m_confusionHardStats;

private slots:
    void resetStats();
};

#endif // STATSTRACKER_H
