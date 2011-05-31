#ifndef ENEMYFACTORY_H
#define ENEMYFACTORY_H

#include <QList>
#include <QString>

#include "map.h"
#include "types.h"
#include "enemy.h"

class Map;

class WaveElement
{
    TYPE::ENEMY _type;
    float _size;
    int _number;
    int _frequence;    

public:

    WaveElement(QString data);
    TYPE::ENEMY getType(void) const;
    float getSize(void) const;
    int getNumber(void) const;
    int getFrequence(void) const;
    void decreaseNumber(void);
};


class Wave
{
    QList< WaveElement* > _waveElements;
    QString _desc;

public:

    Wave(QString data);
    WaveElement* popNextWaveElement(void);
    bool isFinished(void) const;
    QString getDesc(void) const;
};

class EnemyFactory : public QObject
{
    Q_OBJECT

    QList<Wave*> _mapWaves;
    Map* _currentMap;    
    Player* _player;
    float _waveTimerStep;

    void productEnemy(WaveElement* enemy);

public:

    EnemyFactory(Map* map,Player* player);

public slots:
    void getNextWaveDesc(void);
    void launchWaves(void);
    void produceWave(void);

signals:
    void changeWaveDescLabel(QString);
    void setLaunchWaveButtonEnabled(bool);
    void gameWon(QString);

};



#endif // ENEMYFACTORY_H
