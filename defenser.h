#ifndef DEFENSER_H
#define DEFENSER_H

#include "entity.h"

class Defenser : public Entity
{
public:
    Defenser(int posx,int posy,int level,TYPE::ENTITY target);
    virtual int getCost(void) const = 0;

protected:
    int _level;
    TYPE::ENTITY _target;
    int _rate;
    int _range;
    int _power;
};

#endif // DEFENSER_H
