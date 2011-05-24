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
    virtual ~Enemy(void);
    int getResistance(void) const;
    int getHP(void) const;
    float getSize(void) const;
    virtual void hurt(int damages);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);
    QRectF boundingRect(void) const;

protected:
    Map* _map;
    float _size;
    int _hp;
    int _hpMax;
    int _resistance;
    float _speed;

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
    Cafard(Map* map,int posx,int posy, float size);
    ~Cafard(void);
    void hurt(int damages);

protected:
    void advance(int phase);

};


#endif // MONSTER_H

