#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include <QObject>

class LevelGenerator : public QObject
{
    Q_OBJECT
public:
    explicit LevelGenerator(QObject *parent = nullptr);
    slots genera();

signals:

};

#endif // LEVELGENERATOR_H
