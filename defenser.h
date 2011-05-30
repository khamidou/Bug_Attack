#ifndef DEFENSER_H
#define DEFENSER_H

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
    bool isSelected(void) const;
    virtual bool isLevelMax(void) const;
    void setIsShooting(bool state);
    void setTarget(float targetX,float targetY);
    QRectF boundingRect(void) const;
    bool isShooting(void) const;

protected:
    virtual void updateStats(void) = 0;
    virtual void advance(int phase);
    virtual void shootTarget(void) = 0;

    int _shootTimerStep;
    int _level;
    int _targetType; // combinaison de TYPE::ENTITY
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

    void shootTarget(void);

public:

    static const int DEFAULT_TARGET_TYPE = TYPE::T_RAMPANT | TYPE::T_VOLANT;
    static const int BASIC_COST = 8;
    static const int INTERMEDIATE_COST = 20;
    static const int HIGHEST_COST = 45;

    WaterGun(int posx,int posy,int level,Map* map);
    int getCost(int level = -1) const;
    QString getInfos(void) const;   
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);
    static int getBasicCost(void) { return BASIC_COST; }



protected:
    void updateStats(void);
};

/**
* LANCE PIERRE
**/


class Slingshot : public Defenser
{
    Q_OBJECT

    void shootTarget(void);

public:

    static const int DEFAULT_TARGET_TYPE = TYPE::T_VOLANT;
    static const int BASIC_COST = 12;
    static const int INTERMEDIATE_COST = 25;
    static const int HIGHEST_COST = 60;

    Slingshot(int posx,int posy,int level,Map* map);
    int getCost(int level = -1) const;
    QString getInfos(void) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);
    static int getBasicCost(void) { return BASIC_COST; }

protected:
    void updateStats(void);
};

/**
* PAINTBALL
**/


class Paintball : public Defenser
{
    Q_OBJECT

    void shootTarget(void);

public:

    static const int DEFAULT_TARGET_TYPE = TYPE::T_RAMPANT | TYPE::T_VOLANT;
    static const int BASIC_COST = 12;
    static const int INTERMEDIATE_COST = 25;
    static const int HIGHEST_COST = 60;

    Paintball(int posx,int posy,int level,Map* map);
    int getCost(int level = -1) const;
    QString getInfos(void) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);
    static int getBasicCost(void) { return BASIC_COST; }

protected:
    void updateStats(void);
};

/**
* PETANQUE
**/


class Bowling : public Defenser
{
    Q_OBJECT

    void shootTarget(void);

public:

    static const int DEFAULT_TARGET_TYPE = TYPE::T_RAMPANT;
    static const int BASIC_COST = 15;
    static const int INTERMEDIATE_COST = 40;
    static const int HIGHEST_COST = 80;

    Bowling(int posx,int posy,int level,Map* map);
    int getCost(int level = -1) const;
    QString getInfos(void) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);
    static int getBasicCost(void) { return BASIC_COST; }

protected:
    void updateStats(void);
};
#endif // DEFENSER_H
