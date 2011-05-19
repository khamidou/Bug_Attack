#include "defenser.h"

Defenser::Defenser(int posx,int posy,int level,TYPE::ENTITY target)
    :Entity(posx,posy),_level(level),_target(target) {}
