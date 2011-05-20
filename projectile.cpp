#include <iostream>
#include <math.h>
#include "map.h"
#include "projectile.h"

Projectile::Projectile(float posX, float posY, float targetX, float targetY, float speed, int damages)
    : Entity(posX,posY),_speed(speed),_damages(damages),_targetX(targetX),_targetY(targetY)
{
    // Calcul du vecteur de déplacement du point (posX,posY) vers (targetX,targetY)
    // .. Nb : on calcule ici le vecteur normalisé pour appliquer ensuite la vitesse propre à l'objet
    float vx = _targetX - this->x();
    float vy = _targetY - this->y();

    _movementVect.setX(vx/sqrt(vx*vx + vy*vy));
    _movementVect.setY(vy/sqrt(vx*vx + vy*vy));
}

QRectF Projectile::boundingRect() const {
    return QRectF(-5,-5,5,5);
}

void Projectile::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*) {

    painter->setBrush(QBrush(Qt::cyan));
    // Dessine la tourelle
    painter->drawRect(0,0,5,5);


}

void Projectile::advance(int phase) {
    if(!phase)
        return;
    // Déplacement du projectile selon le vecteur de déplacement et la vitesse
    float newx = this->x() + _speed*_movementVect.x();
    float newy = this->y() + _speed*_movementVect.y();

    // std::cout << "I'm still alive mothafucka" << std::endl;

    // Si le projectile est en dehors de l'écran, on le supprime
    if(newx < 0 || newx > 512 || newy < 0 || newy > 512){
        this->scene()->removeItem(this); // TODO VERIFIER Semble faire un appel à delete() de l'objet
        return;
    }
    this->setPos(newx,newy);

}
