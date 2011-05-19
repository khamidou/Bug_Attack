#include <iostream>
#include <math.h>
#include "watergun.h"

WaterGun::WaterGun(int posx,int posy,int level,TYPE::ENTITY target)
    : Defenser(posx,posy,level,target)
{
    updateStats();
    this->setPos(_x,_y);
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
    return QRectF(-100,-100,200,200);
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



}
