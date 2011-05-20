#ifndef MONSTER_H
#define MONSTER_H

#include "entity.h"
#include "projectile.h"


class Monster : public Entity {

public:
    Monster(int posx, int posy, float size);
    int getResistance(void) const;
    int getHP(void) const;

protected:
    float _size;
    int _hp;
    int _resistance;
    float _speed;
};



#endif // MONSTER_H

