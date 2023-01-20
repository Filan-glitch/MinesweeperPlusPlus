#include "statstracker.h"

//Constructer
StatsTracker::StatsTracker()
{
    readStats();
}

//function that read the stats.json file and updates the data members
void StatsTracker::readStats()
{
    //Data handling with stats.json file
    QFile file("stats.json");

    //creating file if not existant
    if(!file.exists()) {
        file.open(QIODevice::WriteOnly);
        file.write("[{\"Mode\": \"Easy\", \"Best Time\": null, \"Total Time\": null, \"Wins\": null, \"Loses\": null, \"Efficiency\": null}, {\"Mode\": \"Intermediate\", \"Best Time\": null, \"Total Time\": null, \"Wins\": null, \"Loses\": null, \"Efficiency\": null}, { \"Mode\": \"Hard\", \"Best Time\": null, \"Total Time\": null, \"Wins\": null, \"Loses\": null, \"Efficiency\": null}, {\"Mode\": \"Confusion\", \"Best Time\": null, \"Total Time\": null, \"Wins\": null, \"Loses\": null, \"Efficiency\": null}]");
        qDebug() << "File was not found and therefor created.";
        file.close();
    }

    //Reading data
    QString jsonData;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonArray jsonArray = doc.array();
    m_easyStats = jsonArray[0].toObject();
    m_intermediateStats = jsonArray[1].toObject();
    m_hardStats = jsonArray[2].toObject();
    m_confusionStats = jsonArray[3].toObject();
}

//function that writes the updated data members to the stats.json file
void StatsTracker::writeStats()
{
    //Data handling with stats.json file
    QFile file("stats.json");

    //deleting old data
    file.resize(0);

    //Writing data
    QJsonArray jsonArray;
    jsonArray.append(m_easyStats);
    jsonArray.append(m_intermediateStats);
    jsonArray.append(m_hardStats);
    jsonArray.append(m_confusionStats);

    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArray);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(jsonDoc.toJson());
    file.close();
    qDebug() << "Statistics were saved.";
}

//getter for stats for easy mode
QJsonObject StatsTracker::easyStats() const
{
    return m_easyStats;
}

//getter for stats for intermediate mode
QJsonObject StatsTracker::intermediateStats() const
{
    return m_intermediateStats;
}

//getter for stats for hard mode
QJsonObject StatsTracker::hardStats() const
{
    return m_hardStats;
}

//getter for stats for confusion mode
QJsonObject StatsTracker::confusionStats() const
{
    return m_confusionStats;
}

//function that updates wins/loses, best time and efficiency
void StatsTracker::roundsPlayedUpdate(GameChoiceDialog::Choice choice, bool win, int time, float efficiency)
{
    switch(choice) {
    case GameChoiceDialog::EASY:
        m_easyStats["Total Time"] = m_easyStats["Total Time"].toInt() + time;
        if(win) {
            m_easyStats["Wins"] = m_easyStats["Wins"].toInt() + 1;

            if(m_easyStats["Best Time"] == QJsonValue()) {
                m_easyStats["Best Time"] = time;
                break;
            }
            if(time < m_easyStats["Best Time"].toInt()) {
                m_easyStats["Best Time"] = time;
            }
        } else {
            m_easyStats["Loses"] = m_easyStats["Loses"].toInt() + 1;
        }
        break;
    case GameChoiceDialog::INTERMEDIATE:
        m_intermediateStats["Total Time"] = m_intermediateStats["Total Time"].toInt() + time;
        if(win) {
            m_intermediateStats["Wins"] = m_intermediateStats["Wins"].toInt() + 1;

            if(m_intermediateStats["Best Time"] == QJsonValue()) {
                m_intermediateStats["Best Time"] = time;
                break;
            }
            if(time < m_intermediateStats["Best Time"].toInt()) {
                m_intermediateStats["Best Time"] = time;
            }
        } else {
            m_intermediateStats["Loses"] = m_intermediateStats["Loses"].toInt() + 1;
        }
        break;
    case GameChoiceDialog::HARD:
        m_hardStats["Total Time"] = m_hardStats["Total Time"].toInt() + time;
        if(win) {
            m_hardStats["Wins"] = m_hardStats["Wins"].toInt() + 1;

            if(m_hardStats["Best Time"] == QJsonValue()) {
                m_hardStats["Best Time"] = time;
                break;
            }

            if(time < m_hardStats["Best Time"].toInt()) {
                m_hardStats["Best Time"] = time;
            }
        } else {
            m_hardStats["Loses"] = m_hardStats["Loses"].toInt() + 1;
        }
        break;
    case GameChoiceDialog::CONFUSION:
        m_confusionStats["Total Time"] = m_confusionStats["Total Time"].toInt() + time;
        if(win) {
            m_confusionStats["Wins"] = m_confusionStats["Wins"].toInt() + 1;

            if(m_confusionStats["Best Time"] == QJsonValue()) {
                m_confusionStats["Best Time"] = time;
                break;
            }
            if(time < m_confusionStats["Best Time"].toInt()) {
                m_confusionStats["Best Time"] = time;
            }
        } else {
            m_confusionStats["Loses"] = m_confusionStats["Loses"].toInt() + 1;
        }
        break;
    default:
        break;
    }
}

void StatsTracker::resetStats()
{
    //Data handling with stats.json file
    QFile file("stats.json");

    //deleting old data
    file.moveToTrash();

    //reset stats.json file
    file.open(QIODevice::WriteOnly);
    file.write("[{\"Mode\": \"Easy\", \"Best Time\": null, \"Total Time\": null, \"Wins\": null, \"Loses\": null, \"Efficiency\": null}, {\"Mode\": \"Intermediate\", \"Best Time\": null, \"Total Time\": null, \"Wins\": null, \"Loses\": null, \"Efficiency\": null}, { \"Mode\": \"Hard\", \"Best Time\": null, \"Total Time\": null, \"Wins\": null, \"Loses\": null, \"Efficiency\": null}, {\"Mode\": \"Confusion\", \"Best Time\": null, \"Total Time\": null, \"Wins\": null, \"Loses\": null, \"Efficiency\": null}]");
    qDebug() << "Statistics were resetted.";
    file.close();

    //reset intern data
    m_easyStats["Best Time"] = QJsonValue();
    m_easyStats["Total Time"] = QJsonValue();
    m_easyStats["Wins"] = QJsonValue();
    m_easyStats["Loses"] = QJsonValue();
    m_easyStats["Efficiency"] = QJsonValue();

    m_intermediateStats["Best Time"] = QJsonValue();
    m_intermediateStats["Total Time"] = QJsonValue();
    m_intermediateStats["Wins"] = QJsonValue();
    m_intermediateStats["Loses"] = QJsonValue();
    m_intermediateStats["Efficiency"] = QJsonValue();

    m_hardStats["Best Time"] = QJsonValue();
    m_hardStats["Total Time"] = QJsonValue();
    m_hardStats["Wins"] = QJsonValue();
    m_hardStats["Loses"] = QJsonValue();
    m_hardStats["Efficiency"] = QJsonValue();

    m_confusionStats["Best Time"] = QJsonValue();
    m_confusionStats["Total Time"] = QJsonValue();
    m_confusionStats["Wins"] = QJsonValue();
    m_confusionStats["Loses"] = QJsonValue();
    m_confusionStats["Efficiency"] = QJsonValue();
}

