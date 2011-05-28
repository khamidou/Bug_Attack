#ifndef DEFENSER_H
#define DEFENSER_H

#include <QTimer>
#include "map.h"
#include "entity.h"

class Map;

/**
* DEFENSER
**/

class Defenser : public Entity
{
    Q_OBJECT

public:
    Defenser(int posx,int posy,int level,Map* map);
    int getLevel(void) const;
    virtual int getCost(int level = -1) const = 0;
    virtual QString getInfos(void) const = 0;
    void increaseLevel(void);
    virtual bool isLevelMax(void) const = 0;
    bool isSelected(void) const;
    void setIsShooting(bool state);

protected:
    virtual void updateStats(void) = 0;

    int _shootTimerStep;
    int _level;
    int _target; // combinaison de TYPE::ENTITY
    float _rate;
    float _range;
    int _power;
    float _targetX;
    float _targetY;
    Map* _map;
    bool _isShooting;
    bool _isSelected;

public slots:
    void setIsSelected(bool selected);

signals:
    void turretRemoved(Defenser*);

};

/**
* PISTOLET A EAU
**/


class WaterGun : public Defenser
{
    Q_OBJECT

public:
    WaterGun(int posx,int posy,int level,Map* map);
    int getCost(int level = -1) const;
    QString getInfos(void) const;
    bool isShooting(void) const;
    bool isLevelMax(void) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);
    void setTarget(float targetX,float targetY);
    QRectF boundingRect(void) const;
    static int getBasicCost(void) { return 8; }


protected:
    void advance(int phase);
    void updateStats(void);

public slots:
    void shootTarget(void);

};

#endif // DEFENSER_H
