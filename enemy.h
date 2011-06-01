/**
* @file enemy.h
* @brief Ce fichier contient toutes les classes d'ennemis du jeu.
* Il est également possible de réaliser ses propres ennemis en dérivant de la classe
* patron "Enemy"
*/

#ifndef MONSTER_H
#define MONSTER_H

#include "map.h"
#include "entity.h"
#include "projectile.h"

class Map;


/**
* ENEMY
**/

class Enemy : public Entity {

    Q_OBJECT

public:
    Enemy(Map* map,int posx, int posy, float size);
    int getResistance(void) const;
    int getHP(void) const;
    TYPE::ENTITY getType(void) const;
    float getSize(void) const;
    virtual void hurt(int damages);
    void setSpeedMalus(float percent,int seconds);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);
    QRectF boundingRect(void) const;

protected:
    Map* _map;
    float _size;
    int _hp;
    int _hpMax;
    int _resistance;
    float _speed;

    // Malus
    float _speedMalus;
    int _malusCounter;

    TYPE::ENTITY _type;

    virtual void advance(int phase);
    void reachGoal(void);


signals:
    void killedByPlayer(int moneyEarned);
    void killedAtGoal(int livesLost);
    void enemyDestroyed(Enemy* ptr);
};


/**
* CAFARD
**/

class Cafard : public Enemy
{


public:

    const static TYPE::ENTITY DEFAULT_TYPE = TYPE::T_RAMPANT;

    Cafard(Map* map,int posx,int posy, float size);
    void hurt(int damages);

protected:
    void advance(int phase);

};

/**
* FOURMI
**/

class Fourmi : public Enemy
{


public:

    const static TYPE::ENTITY DEFAULT_TYPE = TYPE::T_RAMPANT;

    Fourmi(Map* map,int posx,int posy, float size);
    void hurt(int damages);


protected:
    bool _speedUp;
    int _speedUpCounter;

    void advance(int phase);

};

/**
* GUEPE
**/

class Guepe : public Enemy
{


public:

    const static TYPE::ENTITY DEFAULT_TYPE = TYPE::T_VOLANT;

    Guepe(Map* map,int posx,int posy, float size);
    void hurt(int damages);


protected:
    bool _speedUp;
    int _speedUpCounter;

    void advance(int phase);

};

/**
* MOUSTIQUE
**/

class Moustique : public Enemy
{


public:

    const static TYPE::ENTITY DEFAULT_TYPE = TYPE::T_VOLANT;

    Moustique(Map* map,int posx,int posy, float size);
    void hurt(int damages);


protected:
    int _stateCounter;
    void updateStats(void);
    void advance(int phase);

};

#endif // MONSTER_H

