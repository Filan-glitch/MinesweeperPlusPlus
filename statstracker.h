#ifndef STATSTRACKER_H
#define STATSTRACKER_H

#include <QString>

class StatsTracker
{
public:
    StatsTracker();
    void readFile();


private:
    float winrate;
    QString bestTime;

};

#endif // STATSTRACKER_H
