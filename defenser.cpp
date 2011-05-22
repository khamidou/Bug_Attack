#include <iostream>
#include <typeinfo>
#include <math.h>
#include "defenser.h"
#include "projectile.h"

/**
* DEFENSER
**/

Defenser::Defenser(int posx,int posy,int level,TYPE::ENTITY target)
    :Entity(posx,posy),_level(level),_target(target) {}



/**
* PISTOLET A EAU
**/


WaterGun::WaterGun(int posx,int posy,int level,TYPE::ENTITY target,Map* map)
    : Defenser(posx,posy,level,target),_map(map)
{
    this->updateStats();

    QObject::connect(&_shootTimer,SIGNAL(timeout()),this,SLOT(shootTarget()));
}

int WaterGun::getCost(void) const{

    switch(_level) {
        case 1: return 8;
        case 2: return 20;
        default: return 45;
    }
}

bool WaterGun::isShooting(void) const { return _isShooting;}


void WaterGun::updateStats(void) {

    _range = (2 + _level/2)*50;
    _rate = 4 - _level/2;
    _power = 5 * _level * sqrt(_level); // level ^ 1.5

}

QRectF WaterGun::boundingRect(void) const {
    return QRectF(-32,-32,_range,_range);
}

void WaterGun::setTarget(float targetX,float targetY) {
    // Sauvegarde la position de la cible courante
    _targetX = targetX;
    _targetY = targetY;
}

void WaterGun::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*) {

    painter->setBrush(QBrush(Qt::red));
    // Dessine la tourelle
    painter->drawRect(0,0,32,32);

    // Dessine la portée de la tourelle
    painter->setBrush(QBrush(Qt::blue,Qt::NoBrush));
    painter->drawEllipse(-32 - 2,-32 - 2,_range,_range);

}

void WaterGun::advance(int phase) {

    if(!phase)
        return;

    // Détection des ennemis aux alentours
    QList<Enemy*> enemies = _map->getEnemyList();

    QList<Enemy*>::iterator i;

    // Recherche des ennemis de la map
    for(i = enemies.begin() ; i != enemies.end() ; ++i) {

            float targetX = (*i)->x();
            float targetY = (*i)->y();

            if(sqrt(pow(fabs(this->x() - targetX),2)
                    +pow(fabs(this->y() - targetY),2)
                )  <= _range)
            {
                // Si le défenseur ne tirait pas encore
                if(!_isShooting) {
                    this->setIsShooting(true); // Modification via la méthode pour les effets visuels
                }
                // Actualise la position de la cible
                this->setTarget(targetX+(*i)->getSize()*16,targetY+(*i)->getSize()*16);

                return; // On vise la première cible rencontrée

            } // end distanceTest

    } //eof

    // Si aucun monstre n'a été rencontré
    this->setIsShooting(false);  // Modification via la méthode pour les effets visuels

} // eom

void WaterGun::setIsShooting(bool state) {

    _isShooting = state;

    // MODE TIR ACTIVÉ
    if(state)
    {
        // Change l'aspect visuel
        this->setOpacity(0.5f); // TEST = TIR ACTIVÉ
        // Active le timer
        _shootTimer.start(100*_rate);
    }
    // MODE TIR DESACTIVÉ
    else
    {
        // Change l'aspect visuel
        this->setOpacity(1.0f); // TEST = TIR ACTIVÉ
        // Désactive le timer
        _shootTimer.stop();
    }
}

void WaterGun::shootTarget(void) {

    // Créé un nouveau projectile au niveau du centre de la tourelle
    Projectile* shot = new Projectile(this->x()+14,this->y()+14,_targetX,_targetY,5,_power);
    _map->addItem(shot);

}

