#include <iostream>
#include <typeinfo>
#include <math.h>
#include "defenser.h"
#include "projectile.h"

/**
* DEFENSER
**/

Defenser::Defenser(int posx,int posy,int level,TYPE::ENTITY target,Map* map)
    :Entity(posx,posy),_level(level),_target(target),_map(map)
{
    this->setIsSelected(true); // Appel à la méthode pour rafraichir l'affichage
}


void Defenser::setIsSelected(bool selected) {
    _isSelected = selected;
    _map->update(); // Met à jour l'affichage
}

int Defenser::getLevel(void) const { return _level; }
bool Defenser::isSelected(void) const { return _isSelected; }

void Defenser::increaseLevel(void) {
    if(_level < 3) _level++;
    this->updateStats();
}


void Defenser::setIsShooting(bool state) {

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


/**
* PISTOLET A EAU
**/

WaterGun::WaterGun(int posx,int posy,int level,TYPE::ENTITY target,Map* map)
    : Defenser(posx,posy,level,target,map)
{
    this->updateStats();

    QObject::connect(&_shootTimer,SIGNAL(timeout()),this,SLOT(shootTarget()));
}

int WaterGun::getCost(int level) const{

    // Si aucune valeur en argument, alors on affiche le coût du niveau courant
    if(level == -1) level = _level;

    switch(level) {
        case 1: return 8;
        case 2: return 20;
        default: return 45;
    }
}

QString WaterGun::getInfos(void) const {

    QString infos = "Vous avez clique sur un pistolet a eau de niveau ";
    infos+= QString::number(_level);
    infos+= "\n- Amelioration : ";
    infos+= QString::number(this->getCost(_level+1));
    infos+= "\n- Revente : ";
    infos+= QString::number(this->getCost(_level)/2);

    return infos;
}

bool WaterGun::isShooting(void) const { return _isShooting;}

bool WaterGun::isLevelMax(void) const { return _level==3; }

void WaterGun::updateStats(void) {

    _range = (2.0f + _level/2.0f)*32.0f;
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

    // Si la tourelle est selectionnée, on dessine un halo autour
    if(_isSelected) {
        painter->setBrush(QBrush(Qt::yellow));
        painter->drawRect(-2,-2,36,36);
    }

    // Dessine la tourelle
    painter->setBrush(QBrush(Qt::red));
    painter->drawRect(0,0,32,32);

    // Dessine la portée de la tourelle
    painter->setBrush(QBrush(Qt::blue,Qt::NoBrush));
    painter->drawEllipse((-_range/32)*16+16,(-_range/32)*16+16,_range,_range);

    _map->update();

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

            if(sqrt(pow(fabs(this->x()-16 - targetX - ((*i)->getSize()-1)*16),2)
                    +pow(fabs(this->y()-16 - targetY - ((*i)->getSize()-1)*16),2)
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

void WaterGun::shootTarget(void) {

    // Créé un nouveau projectile au niveau du centre de la tourelle
    Projectile* shot = new Projectile(this->x()+14,this->y()+14,_targetX,_targetY,5,_power, _map);
    _map->addItem(shot);

}

