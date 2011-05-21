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

public:
    Enemy(int posx, int posy, float size);
    int getResistance(void) const;
    int getHP(void) const;
    float getSize(void) const;
    void hurt(int damages);

protected:
    float _size;
    int _hp;
    int _resistance;
    float _speed;
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

    Map* _map;
};


#endif // MONSTER_H

