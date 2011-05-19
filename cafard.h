#ifndef CAFARD_H
#define CAFARD_H

#include "entity.h"
#include "monster.h"
#include "map.h"

class Map;

class Cafard : public Monster
{
public:
    Cafard(Map* map,int posx,int posy, int size);

protected:
    void advance(int phase);

    Map* _map;
};

#endif // CAFARD_H
