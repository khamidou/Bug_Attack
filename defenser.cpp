#include <math.h>
#include "defenser.h"
#include "projectile.h"
#include <iostream>

/**
* DEFENSER
**/

Defenser::Defenser(int posx,int posy,int level,Map* map)
    :Entity(posx,posy),_level(level),_map(map)
{
    this->setIsSelected(true); // Appel à la méthode pour rafraichir l'affichage
    // Met le compteur interne à 0 et relie le FPS au tir
    _shootTimerStep = 0;
    // QObject::connect(&map->gameTimer,SIGNAL(timeout()), this,SLOT(shootTarget()));
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

void Defenser::advance(int phase) {
    if(!phase)
        return;

    // Détection des ennemis aux alentours
    QList<Enemy*> enemies = _map->getEnemyList();

    QList<Enemy*>::iterator i;

    // Recherche des ennemis de la map (tire sur tous les types d'ennemis)
    for(i = enemies.begin() ; i != enemies.end() ; ++i) {

            float targetX = (*i)->x();
            float targetY = (*i)->y();

            if(    (_targetType | (*i)->getType()) == _targetType &&
                    sqrt(pow(fabs(this->x()-16 - targetX - ((*i)->getSize()-1)*16),2)
                    +pow(fabs(this->y()-16 - targetY - ((*i)->getSize()-1)*16),2)
                )  <= _range)
            {
                // Si le défenseur ne tirait pas encore
                if(!_isShooting) {
                    this->setIsShooting(true); // Modification via la méthode pour les effets visuels
                }
                // Actualise la position de la cible
                this->setTarget(targetX+(*i)->getSize()*16,targetY+(*i)->getSize()*16);
                // Tire
                this->shootTarget();

                return; // On vise la première cible rencontrée

            } // end distanceTest

    } //eof
    // Si aucun monstre n'a été rencontré
    this->setIsShooting(false);  // Modification via la méthode pour les effets visuels

} // eom

QRectF Defenser::boundingRect(void) const {
    return QRectF(-32,-32,_range,_range);
}

void Defenser::setTarget(float targetX,float targetY) {
    // Sauvegarde la position de la cible courante
    _targetX = targetX;
    _targetY = targetY;
}

void Defenser::setIsShooting(bool state) {

    _isShooting = state;

    // MODE TIR ACTIVÉ
    if(state)
    {
        // Change l'aspect visuel
        this->setOpacity(0.5f); // TEST = TIR ACTIVÉ
        // Active le timer
        //_shootTimer.start(100*_rate);
    }
    // MODE TIR DESACTIVÉ
    else
    {
        // Change l'aspect visuel
        this->setOpacity(1.0f); // TEST = TIR ACTIVÉ
        // Désactive le timer
        //_shootTimer.stop();
    }
}


bool Defenser::isShooting(void) const { return _isShooting;}

// On considère que par défaut seules trois améliorations sont possibles
// Cette méthode est néanmoins virtuelle pour cas particulier(s)
bool Defenser::isLevelMax(void) const { return _level==3; }

/**
* PISTOLET A EAU
**/

WaterGun::WaterGun(int posx,int posy,int level,Map* map)
    : Defenser(posx,posy,level,map)
{
    // Type de cible
    _targetType = DEFAULT_TARGET_TYPE;
    // Caractéristiques
    this->updateStats();
}

int WaterGun::getCost(int level) const{

    // Si aucune valeur en argument, alors on affiche le coût du niveau courant
    if(level == -1) level = _level;

    switch(level) {
        case 1: return BASIC_COST;
        case 2: return INTERMEDIATE_COST;
        default: return HIGHEST_COST;
    }
}

QString WaterGun::getInfos(void) const {

    // Infos générales
    QString infos = "Pistolet a eau de niveau ";
    infos+= QString::number(_level);
    // Amélioration
    if(!this->isLevelMax()) {
        infos+= "\n- Amelioration : ";
        infos+= QString::number(this->getCost(_level+1));
    }
    // Revente
    infos+= "\n- Revente : ";
    infos+= QString::number(this->getCost(_level)/2);

    return infos;
}


void WaterGun::updateStats(void) {

    _range = (2.0f + _level/2.0f)*32.0f;
    _rate = 4 - _level/2;
    _power = 5 * _level * sqrt(_level); // level ^ 1.5

}


void WaterGun::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*) {

    // Si la tourelle est selectionnée, on dessine un halo autour
    if(_isSelected) {
        painter->setBrush(QBrush(Qt::yellow));
        painter->drawRect(-2,-2,36,36);
    }

    // Dessine la tourelle
    painter->setBrush(QBrush(Qt::blue));
    painter->drawRect(0,0,32,32);

    // Dessine la portée de la tourelle
    painter->setBrush(QBrush(Qt::blue,Qt::NoBrush));
    painter->drawEllipse((-_range/32)*16+16,(-_range/32)*16+16,_range,_range);
}

void WaterGun::shootTarget(void) {
    if (!isShooting())
        return;

    _shootTimerStep+= 1;

    if (_shootTimerStep < (GAME::FPS / _rate))
        return;

    _shootTimerStep = 0;

    // Créé un nouveau projectile au niveau du centre de la tourelle
    Projectile* shot = new ProjectileWater(this->x()+14,this->y()+14,_targetX,_targetY,_power, _map);
    _map->addItem(shot);

}


/**
* LANCE PIERRE
**/

Slingshot::Slingshot(int posx,int posy,int level,Map* map)
    : Defenser(posx,posy,level,map)
{
    // Type de cible
    _targetType = DEFAULT_TARGET_TYPE;
    _shootTimerStep = 0;
    // Caractéristiques
    this->updateStats();
}

int Slingshot::getCost(int level) const{

    // Si aucune valeur en argument, alors on affiche le coût du niveau courant
    if(level == -1) level = _level;

    switch(level) {
        case 1: return BASIC_COST;
        case 2: return INTERMEDIATE_COST;
        default: return HIGHEST_COST;
    }
}

QString Slingshot::getInfos(void) const {

    // Infos générales
    QString infos = "Lance-pierres de niveau ";
    infos+= QString::number(_level);
    // Amélioration
    if(!this->isLevelMax()) {
        infos+= "\n- Amelioration : ";
        infos+= QString::number(this->getCost(_level+1));
    }
    // Revente
    infos+= "\n- Revente : ";
    infos+= QString::number(this->getCost(_level)/2);

    return infos;
}



void Slingshot::updateStats(void) {

    _range = (3.0f + _level/2.0f)*32.0f;
    _rate = 1;
    _power = 10 * _level * sqrt(_level); // level ^ 1.5

}



void Slingshot::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*) {

    // Si la tourelle est selectionnée, on dessine un halo autour
    if(_isSelected) {
        painter->setBrush(QBrush(Qt::yellow));
        painter->drawRect(-2,-2,36,36);
    }

    // Dessine la tourelle
    painter->setBrush(QBrush(Qt::gray));
    painter->drawRect(0,0,32,32);

    // Dessine la portée de la tourelle
    painter->setBrush(QBrush(Qt::blue,Qt::NoBrush));
    painter->drawEllipse((-_range/32)*16+16,(-_range/32)*16+16,_range,_range);
}

void Slingshot::shootTarget(void) {
    if (!isShooting())
        return;

    _shootTimerStep+= 1;

    if (_shootTimerStep < (GAME::FPS / _rate))
        return;

    _shootTimerStep = 0;

    // Créé un nouveau projectile au niveau du centre de la tourelle
    Projectile* shot = new ProjectileStone(this->x()+14,this->y()+14,_targetX,_targetY,_power, _map);
    _map->addItem(shot);

}


/**
* PAINTBALL
**/

Paintball::Paintball(int posx,int posy,int level,Map* map)
    : Defenser(posx,posy,level,map)
{
    // Type de cible
    _targetType = DEFAULT_TARGET_TYPE;
    _shootTimerStep = 0;
    // Caractéristiques
    this->updateStats();
}

int Paintball::getCost(int level) const{

    // Si aucune valeur en argument, alors on affiche le coût du niveau courant
    if(level == -1) level = _level;

    switch(level) {
        case 1: return BASIC_COST;
        case 2: return INTERMEDIATE_COST;
        default: return HIGHEST_COST;
    }
}

QString Paintball::getInfos(void) const {

    // Infos générales
    QString infos = "Paint-ball de niveau ";
    infos+= QString::number(_level);
    // Amélioration
    if(!this->isLevelMax()) {
        infos+= "\n- Amelioration : ";
        infos+= QString::number(this->getCost(_level+1));
    }
    // Revente
    infos+= "\n- Revente : ";
    infos+= QString::number(this->getCost(_level)/2);

    return infos;
}



void Paintball::updateStats(void) {

    _range = (4.0f + _level/2.0f)*32.0f;
    _rate = 2;
    _power = 4 * _level * sqrt(_level); // level ^ 1.5

}



void Paintball::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*) {

    // Si la tourelle est selectionnée, on dessine un halo autour
    if(_isSelected) {
        painter->setBrush(QBrush(Qt::yellow));
        painter->drawRect(-2,-2,36,36);
    }

    // Dessine la tourelle
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(0,0,32,32);

    // Dessine la portée de la tourelle
    painter->setBrush(QBrush(Qt::blue,Qt::NoBrush));
    painter->drawEllipse((-_range/32)*16+16,(-_range/32)*16+16,_range,_range);
}

void Paintball::shootTarget(void) {
    if (!isShooting())
        return;

    _shootTimerStep+= 1;

    if (_shootTimerStep < (GAME::FPS / _rate))
        return;

    _shootTimerStep = 0;

    // Créé un nouveau projectile au niveau du centre de la tourelle
    Projectile* shot = new ProjectilePaintball(this->x()+14,this->y()+14,_targetX,_targetY,_power, _map);
    _map->addItem(shot);

}



/**
* PAINTBALL
**/

Bowling::Bowling(int posx,int posy,int level,Map* map)
    : Defenser(posx,posy,level,map)
{
    // Type de cible
    _targetType = DEFAULT_TARGET_TYPE;
    _shootTimerStep = 0;
    // Caractéristiques
    this->updateStats();
}

int Bowling::getCost(int level) const{

    // Si aucune valeur en argument, alors on affiche le coût du niveau courant
    if(level == -1) level = _level;

    switch(level) {
        case 1: return BASIC_COST;
        case 2: return INTERMEDIATE_COST;
        default: return HIGHEST_COST;
    }
}

QString Bowling::getInfos(void) const {

    // Infos générales
    QString infos = "Joueur de petanque de niveau ";
    infos+= QString::number(_level);
    // Amélioration
    if(!this->isLevelMax()) {
        infos+= "\n- Amelioration : ";
        infos+= QString::number(this->getCost(_level+1));
    }
    // Revente
    infos+= "\n- Revente : ";
    infos+= QString::number(this->getCost(_level)/2);

    return infos;
}



void Bowling::updateStats(void) {

    _range = (3.0f + _level/2.0f)*32.0f;
    _rate = 0.5;
    _power = 15 * _level * sqrt(_level); // level ^ 1.5

}



void Bowling::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*) {

    // Si la tourelle est selectionnée, on dessine un halo autour
    if(_isSelected) {
        painter->setBrush(QBrush(Qt::yellow));
        painter->drawRect(-2,-2,36,36);
    }

    // Dessine la tourelle
    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(0,0,32,32);

    // Dessine la portée de la tourelle
    painter->setBrush(QBrush(Qt::blue,Qt::NoBrush));
    painter->drawEllipse((-_range/32)*16+16,(-_range/32)*16+16,_range,_range);
}

void Bowling::shootTarget(void) {
    if (!isShooting())
        return;

    _shootTimerStep+= 1;

    if (_shootTimerStep < (GAME::FPS / _rate))
        return;

    _shootTimerStep = 0;

    // Créé un nouveau projectile au niveau du centre de la tourelle
    Projectile* shot = new ProjectileBowling(this->x()+14,this->y()+14,_targetX,_targetY,_power, _map);
    _map->addItem(shot);

}





