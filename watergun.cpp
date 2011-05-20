#include <iostream>
#include <typeinfo>
#include <math.h>
#include "projectile.h"
#include "watergun.h"

WaterGun::WaterGun(int posx,int posy,int level,TYPE::ENTITY target,Map* map)
    : Defenser(posx,posy,level,target),_map(map)
{
    this->updateStats();
}

int WaterGun::getCost(void) const{

    switch(_level) {
        case 1: return 8;
        case 2: return 20;
        default: return 45;
    }
}

void WaterGun::updateStats(void) {

    _range = (2 + _level/2)*50;
    _rate = 4 - _level/2;
    _power = 5 * _level * sqrt(_level); // level ^ 1.5

}

QRectF WaterGun::boundingRect() const {
    return QRectF(-32,-32,_range,_range);
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

    this->setOpacity(1.0f);

    // Détection des ennemis aux alentours
    QList<QGraphicsItem*> entities = _map->items();

    QList<QGraphicsItem*>::iterator i;

    // Recherche des ennemis de la map
    for(i = entities.begin() ; i != entities.end() ; ++i) {
        if(dynamic_cast<Monster*>(*i) != 0) {

            float targetX = (*i)->x();
            float targetY = (*i)->y();

            if(sqrt(pow(fabs(this->x() - targetX),2)
                    +pow(fabs(this->y() - targetY),2)
                )  <= _range)
            {
                this->setOpacity(0.5f); // TEST = TIR ACTIVÉ
                this->shootAt(targetX,targetY);
                return; // On vise la première cible rencontrée
            } // end distanceTest
        } // end isMonster
    } //eof

} // eom

void WaterGun::shootAt(float targetX,float targetY) {

    // Créé un nouveau projectile au niveau de la tourelle
    Projectile* shot = new Projectile(this->x(),this->y(),targetX,targetY,5,_power);
    _map->addItem(shot);

}
