#include "entity.h"

Entity::Entity(int posX, int posY): _x(posX),_y(posY) {
}

int Entity::getX(void) const { return _x; }
int Entity::getY(void) const { return _y; }

void Entity::increaseAnimationStep(void) {

    // Avance à la frame suivante de l'animation
    _animState++;
    if(_animState >= _animPixmap.size())
        _animState = 0;

}
