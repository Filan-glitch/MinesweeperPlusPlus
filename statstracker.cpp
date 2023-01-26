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
        file.write("[\
                   {\
                       \"Best Time\": null,\
                       \"Efficiency\": null,\
                       \"Loses\": null,\
                       \"Mode\": \"Easy\",\
                       \"Total Time\": null,\
                       \"Wins\": null\
                   },\
                   {\
                       \"Best Time\": null,\
                       \"Efficiency\": null,\
                       \"Loses\": null,\
                       \"Mode\": \"Intermediate\",\
                       \"Total Time\": null,\
                       \"Wins\": null\
                   },\
                   {\
                       \"Best Time\": null,\
                       \"Efficiency\": null,\
                       \"Loses\": null,\
                       \"Mode\": \"Hard\",\
                       \"Total Time\": null,\
                       \"Wins\": null\
                   },\
                   {\
                       \"Best Time\": null,\
                       \"Efficiency\": null,\
                       \"Loses\": null,\
                       \"Mode\": \"ConfusionEasy\",\
                       \"Total Time\": null,\
                       \"Wins\": null\
                   },\
                   {\
                       \"Best Time\": null,\
                       \"Efficiency\": null,\
                       \"Loses\": null,\
                       \"Mode\": \"ConfusionIntermediate\",\
                       \"Total Time\": null,\
                       \"Wins\": null\
                   },\
                   {\
                       \"Best Time\": null,\
                       \"Efficiency\": null,\
                       \"Loses\": null,\
                       \"Mode\": \"ConfusionHard\",\
                       \"Total Time\": null,\
                       \"Wins\": null\
                   },\
                   {\
                       \"Golden Flags\": null\
                   }\
               ]");
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
    m_confusionEasyStats = jsonArray[3].toObject();
    m_confusionIntermediateStats = jsonArray[4].toObject();
    m_confusionHardStats = jsonArray[5].toObject();
    m_amountGoldenFlags = jsonArray[6].toObject();
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
    jsonArray.append(m_confusionEasyStats);
    jsonArray.append(m_confusionIntermediateStats);
    jsonArray.append(m_confusionHardStats);
    jsonArray.append(m_amountGoldenFlags);

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
QJsonObject StatsTracker::confusionEasyStats() const
{
    return m_confusionEasyStats;
}

//getter for stats for confusion mode
QJsonObject StatsTracker::confusionIntermediateStats() const
{
    return m_confusionIntermediateStats;
}

//getter for stats for confusion mode
QJsonObject StatsTracker::confusionHardStats() const
{
    return m_confusionHardStats;
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
    case GameChoiceDialog::CONFUSION1:
        m_confusionEasyStats["Total Time"] = m_confusionEasyStats["Total Time"].toInt() + time;
        if(win) {
            m_confusionEasyStats["Wins"] = m_confusionEasyStats["Wins"].toInt() + 1;

            if(m_confusionEasyStats["Best Time"] == QJsonValue()) {
                m_confusionEasyStats["Best Time"] = time;
                break;
            }
            if(time < m_confusionEasyStats["Best Time"].toInt()) {
                m_confusionEasyStats["Best Time"] = time;
            }
        } else {
            m_confusionEasyStats["Loses"] = m_confusionEasyStats["Loses"].toInt() + 1;
        }
        break;
    case GameChoiceDialog::CONFUSION2:
        m_confusionIntermediateStats["Total Time"] = m_confusionIntermediateStats["Total Time"].toInt() + time;
        if(win) {
            m_confusionIntermediateStats["Wins"] = m_confusionIntermediateStats["Wins"].toInt() + 1;

            if(m_confusionIntermediateStats["Best Time"] == QJsonValue()) {
                m_confusionIntermediateStats["Best Time"] = time;
                break;
            }
            if(time < m_confusionIntermediateStats["Best Time"].toInt()) {
                m_confusionIntermediateStats["Best Time"] = time;
            }
        } else {
            m_confusionIntermediateStats["Loses"] = m_confusionIntermediateStats["Loses"].toInt() + 1;
        }
        break;
    case GameChoiceDialog::CONFUSION3:
        m_confusionHardStats["Total Time"] = m_confusionHardStats["Total Time"].toInt() + time;
        if(win) {
            m_confusionHardStats["Wins"] = m_confusionHardStats["Wins"].toInt() + 1;

            if(m_confusionHardStats["Best Time"] == QJsonValue()) {
                m_confusionHardStats["Best Time"] = time;
                break;
            }
            if(time < m_confusionHardStats["Best Time"].toInt()) {
                m_confusionHardStats["Best Time"] = time;
            }
        } else {
            m_confusionHardStats["Loses"] = m_confusionHardStats["Loses"].toInt() + 1;
        }
        break;
    default:
        break;
    }
}

//getter for amount of golden flags
QJsonObject StatsTracker::amountGoldenFlags() const
{
    return m_amountGoldenFlags;
}

//function that adds a golden flag
void StatsTracker::addGoldenFlag() {
    m_amountGoldenFlags["Golden Flags"] = QJsonValue(m_amountGoldenFlags["Golden Flags"].toInt() + 1);
}

//function that uses a golden flag
void StatsTracker::useGoldenFlag() {
    if(m_amountGoldenFlags["Golden Flags"] != QJsonValue()) m_amountGoldenFlags["Golden Flags"] = QJsonValue(m_amountGoldenFlags["Golden Flags"].toInt() - 1);
}

//function that resets the intern and extern stats
void StatsTracker::resetStats(bool resetGoldenFlags)
{
    //Data handling with stats.json file
    QFile file("stats.json");

    //deleting old data
    file.moveToTrash();

    //reset stats.json file
    file.open(QIODevice::WriteOnly);
    file.write("[\
               {\
                   \"Best Time\": null,\
                   \"Efficiency\": null,\
                   \"Loses\": null,\
                   \"Mode\": \"Easy\",\
                   \"Total Time\": null,\
                   \"Wins\": null\
               },\
               {\
                   \"Best Time\": null,\
                   \"Efficiency\": null,\
                   \"Loses\": null,\
                   \"Mode\": \"Intermediate\",\
                   \"Total Time\": null,\
                   \"Wins\": null\
               },\
               {\
                   \"Best Time\": null,\
                   \"Efficiency\": null,\
                   \"Loses\": null,\
                   \"Mode\": \"Hard\",\
                   \"Total Time\": null,\
                   \"Wins\": null\
               },\
               {\
                   \"Best Time\": null,\
                   \"Efficiency\": null,\
                   \"Loses\": null,\
                   \"Mode\": \"ConfusionEasy\",\
                   \"Total Time\": null,\
                   \"Wins\": null\
               },\
               {\
                   \"Best Time\": null,\
                   \"Efficiency\": null,\
                   \"Loses\": null,\
                   \"Mode\": \"ConfusionIntermediate\",\
                   \"Total Time\": null,\
                   \"Wins\": null\
               },\
               {\
                   \"Best Time\": null,\
                   \"Efficiency\": null,\
                   \"Loses\": null,\
                   \"Mode\": \"ConfusionHard\",\
                   \"Total Time\": null,\
                   \"Wins\": null\
               },\
               {\
                   \"Golden Flags\": null\
               }\
           ]");
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

    m_confusionEasyStats["Best Time"] = QJsonValue();
    m_confusionEasyStats["Total Time"] = QJsonValue();
    m_confusionEasyStats["Wins"] = QJsonValue();
    m_confusionEasyStats["Loses"] = QJsonValue();
    m_confusionEasyStats["Efficiency"] = QJsonValue();

    m_confusionIntermediateStats["Best Time"] = QJsonValue();
    m_confusionIntermediateStats["Total Time"] = QJsonValue();
    m_confusionIntermediateStats["Wins"] = QJsonValue();
    m_confusionIntermediateStats["Loses"] = QJsonValue();
    m_confusionIntermediateStats["Efficiency"] = QJsonValue();

    m_confusionHardStats["Best Time"] = QJsonValue();
    m_confusionHardStats["Total Time"] = QJsonValue();
    m_confusionHardStats["Wins"] = QJsonValue();
    m_confusionHardStats["Loses"] = QJsonValue();
    m_confusionHardStats["Efficiency"] = QJsonValue();

    if(resetGoldenFlags) m_amountGoldenFlags["Golden Flags"] = QJsonValue();
}

