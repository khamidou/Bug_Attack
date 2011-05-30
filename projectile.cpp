#include <iostream>
#include <math.h>
#include "map.h"
#include "projectile.h"

/**
* PROJECTILE
**/

Projectile::Projectile(float posX, float posY, float targetX, float targetY, int damages, Map* map)
    : Entity(posX,posY),_damages(damages),_targetX(targetX),_targetY(targetY)
{
    // Calcul du vecteur de déplacement du point (posX,posY) vers (targetX,targetY)
    // .. Nb : on calcule ici le vecteur normalisé pour appliquer ensuite la vitesse propre à l'objet
    float vx = _targetX - this->x();
    float vy = _targetY - this->y();

    _movementVect.setX(vx/sqrt(vx*vx + vy*vy));
    _movementVect.setY(vy/sqrt(vx*vx + vy*vy));
    _map = map;
}

QRectF Projectile::boundingRect() const {
    return QRectF(0,0,5,5);
}

void Projectile::advance(int phase) {

    if(!phase)
        return;

    // Collision avec ennemi
    // . Recherche des ennemis de la map
    QList<Enemy*> entities = _map->getEnemyList();
    QList<Enemy*>::iterator i;
    for(i = entities.begin(); i < entities.end() ; ++i) {
            if(((*i)->getType() | _targetType) == _targetType &&
               this->collidesWithItem(*i,Qt::IntersectsItemBoundingRect)){
                // On retire le projectile de la scène
                this->scene()->removeItem(this);
                // Inflige des domages au monstre
                Enemy *e = *i;
                e->hurt(_damages);
                return;
            }
    }



    // Déplacement du projectile selon le vecteur de déplacement et la vitesse
    float newx = this->x() + _speed*_movementVect.x();
    float newy = this->y() + _speed*_movementVect.y();

    // std::cout << "I'm still alive mothafucka" << std::endl;

    // Si le projectile est en dehors de l'écran, on le supprime
    if(newx < 0 || newx > 512 || newy < 0 || newy > 512){
        _map->removeItem(this); // TODO VERIFIER Semble faire un appel à delete() de l'objet
        return;
    }
    this->setPos(newx,newy);

}

/**
* PROJECTILE WATER
**/

ProjectileWater::ProjectileWater(float posX,float posY,float targetX,float targetY,int damages, Map *map)
    :Projectile(posX,posY,targetX,targetY,damages,map){
    _targetType = DEFAULT_TARGET_TYPE;
    _speed = DEFAULT_SPEED;
}

void ProjectileWater::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*) {
    // Dessine le projectile
    painter->setBrush(QBrush(Qt::cyan));
    painter->drawRect(0,0,5,5);
}

/**
* PROJECTILE WATER
**/

ProjectileStone::ProjectileStone(float posX,float posY,float targetX,float targetY,int damages, Map *map)
    :Projectile(posX,posY,targetX,targetY,damages,map){
    _targetType = DEFAULT_TARGET_TYPE;
    _speed = DEFAULT_SPEED;
}

void ProjectileStone::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*) {
    // Dessine le projectile
    painter->setBrush(QBrush(Qt::gray));
    painter->drawRect(0,0,5,5);
}


/**
* PROJECTILE PAINT BALL
**/

ProjectilePaintball::ProjectilePaintball(float posX,float posY,float targetX,float targetY,int damages, Map *map)
    :Projectile(posX,posY,targetX,targetY,damages,map){
    _targetType = DEFAULT_TARGET_TYPE;
    _speed = DEFAULT_SPEED;
}

void ProjectilePaintball::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*) {
    // Dessine le projectile
    painter->setBrush(QBrush(Qt::green));
    painter->drawRect(0,0,5,5);
}

void ProjectilePaintball::advance(int phase) {

    if(!phase)
        return;

    // Collision avec ennemi
    // . Recherche des ennemis de la map
    QList<Enemy*> entities = _map->getEnemyList();
    QList<Enemy*>::iterator i;
    for(i = entities.begin(); i < entities.end() ; ++i) {
            if( ((*i)->getType() | _targetType) == _targetType
                && this->collidesWithItem(*i,Qt::IntersectsItemBoundingRect)){
                // On retire le projectile de la scène
                this->scene()->removeItem(this);
                // Inflige des domages au monstre
                Enemy *e = *i;
                e->hurt(_damages);
                // Ralenti également le monstre pendant 4 secondes
                e->setSpeedMalus(0.5f,4);
                return;
            }
    }



    // Déplacement du projectile selon le vecteur de déplacement et la vitesse
    float newx = this->x() + _speed*_movementVect.x();
    float newy = this->y() + _speed*_movementVect.y();

    // std::cout << "I'm still alive mothafucka" << std::endl;

    // Si le projectile est en dehors de l'écran, on le supprime
    if(newx < 0 || newx > 512 || newy < 0 || newy > 512){
        _map->removeItem(this); // TODO VERIFIER Semble faire un appel à delete() de l'objet
        return;
    }
    this->setPos(newx,newy);

}


/**
* PROJECTILE BOWLING
**/

ProjectileBowling::ProjectileBowling(float posX,float posY,float targetX,float targetY,int damages, Map *map)
    :Projectile(posX,posY,targetX,targetY,damages,map){
    _targetType = DEFAULT_TARGET_TYPE;
    _speed = DEFAULT_SPEED;
}

void ProjectileBowling::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*) {
    // Dessine le projectile
    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(0,0,5,5);
}



/**
* Dommages de zone :
* Les dommages de bases sont calculés de la sorte :
* 15 * niveau ^ 1.5
* Les dommages de zone sont calculés de la sorte :
* 10 * niveau ^ 1.5
* Afin d'économiser en calculs, on se base sur les dommages de base, multipliés par 10/15.
**/
void ProjectileBowling::advance(int phase) {

    if(!phase)
        return;

    // Collision avec ennemi
    // . Recherche des ennemis de la map
    QList<Enemy*> entities = _map->getEnemyList();
    QList<Enemy*>::iterator i;
    for(i = entities.begin(); i < entities.end() ; ++i) {
            if( ((*i)->getType() | _targetType) == _targetType &&
                this->collidesWithItem(*i,Qt::IntersectsItemBoundingRect)) {


                // Inflige des dommages à la cible...
                Enemy *e = *i;
                e->hurt(_damages);

                // ... puis on recherche des ennemis à une case de distance qui seront également blessés
                for(int j = 0 ; j < entities.size() ; ++j) {

                    float targetX = entities[j]->x();
                    float targetY = entities[j]->y();

                    if(entities[j] != e && sqrt(pow(fabs(this->x()-16 - targetX - ((*i)->getSize()-1)*16),2)
                            +pow(fabs(this->y()-16 - targetY - ((*i)->getSize()-1)*16),2)
                        )  <= 32)
                    {
                        entities[j]->hurt(_damages*10/15);
                    }

                }
                // On retire le projectile de la scène
                this->scene()->removeItem(this);

                return;
            }
    }



    // Déplacement du projectile selon le vecteur de déplacement et la vitesse
    float newx = this->x() + _speed*_movementVect.x();
    float newy = this->y() + _speed*_movementVect.y();

    // std::cout << "I'm still alive mothafucka" << std::endl;

    // Si le projectile est en dehors de l'écran, on le supprime
    if(newx < 0 || newx > 512 || newy < 0 || newy > 512){
        _map->removeItem(this); // TODO VERIFIER Semble faire un appel à delete() de l'objet
        return;
    }
    this->setPos(newx,newy);

}
