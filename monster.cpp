#include "monster.h"

Monster::Monster(int posx,int posy, float size):Entity(posx,posy),_size(size){ }


int Monster::getHP(void) const { return _hp; }
int Monster::getResistance(void) const { return _resistance; }

