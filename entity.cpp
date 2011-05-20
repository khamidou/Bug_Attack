#include "entity.h"

Entity::Entity(float posX, float posY) {
    this->setPos(posX,posY);
}


void Entity::increaseAnimationStep(void) {

    // Avance à la frame suivante de l'animation
    _animState++;
    if(_animState >= _animPixmap.size())
        _animState = 0;

}
