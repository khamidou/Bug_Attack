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
    bool isDestroyed(void) const;
    float getSize(void) const;
    void hurt(int damages);

protected:
    Map* _map;
    float _size;
    int _hp;
    int _resistance;
    float _speed;
    bool _destroyed;


signals:
    void killedByPlayer(int moneyEarned);
    void enemyDestroyed(void);
};


/**
* CAFARD
**/

class Cafard : public Enemy
{


public:
    Cafard(Map* map,int posx,int posy, float size);
    ~Cafard(void);

protected:
    void advance(int phase);

};


#endif // MONSTER_H

