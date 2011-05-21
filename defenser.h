#ifndef DEFENSER_H
#define DEFENSER_H

#include <QTimer>
#include "map.h"
#include "entity.h"

class Map;

/**
* DEFENSER
**/

class Defenser : public Entity,public QObject
{
public:
    Defenser(int posx,int posy,int level,TYPE::ENTITY target);
    virtual int getCost(void) const = 0;

protected:
    QTimer _shootTimer;
    int _level;
    TYPE::ENTITY _target;
    int _rate;
    int _range;
    int _power;
    float _targetX;
    float _targetY;
    bool _isShooting;
};

/**
* PISTOLET A EAU
**/


class WaterGun : public Defenser
{

    Q_OBJECT

public:
    WaterGun(int posx,int posy,int level,TYPE::ENTITY target,Map* map);
    int getCost(void) const;
    bool isShooting(void) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);
    void setTarget(float targetX,float targetY);
    void setIsShooting(bool state);
    QRectF boundingRect() const;


protected:
    void advance(int phase);
    void updateStats(void);
    Map* _map;

public slots:
    void shootTarget(void);

};

#endif // DEFENSER_H
