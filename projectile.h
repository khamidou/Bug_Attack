/**
* @file projectile.h
* @brief La définition d'un projectile
*/
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QPainter>
#include "entity.h"
#include "map.h"

/**
* PROJECTILE
**/

class Map;
class Projectile : public Entity
{

protected:

   float _speed;
   int _damages;
   float _targetX;
   float _targetY;
   int _targetType; // Combinaison de TYPE::ENTITY
   QPointF _movementVect;
   Map* _map;

   virtual void advance(int phase);

public:
   Projectile(float posX,float posY,float targetX,float targetY,int damages, Map *map);
   QRectF boundingRect() const;
   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*) = 0;



};


/**
* PROJECTILE WATER
**/

class ProjectileWater : public Projectile {

public:

    // Caractéristiques
    static const float DEFAULT_SPEED = 4;
    static const int DEFAULT_TARGET_TYPE = TYPE::T_RAMPANT | TYPE::T_VOLANT;

    ProjectileWater(float posX,float posY,float targetX,float targetY,int damages, Map *map);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);

};


/**
* PROJECTILE STONE
**/

class ProjectileStone : public Projectile {

public:

    // Caractéristiques
    static const float DEFAULT_SPEED = 2.5;
    static const int DEFAULT_TARGET_TYPE = TYPE::T_VOLANT;


    ProjectileStone(float posX,float posY,float targetX,float targetY,int damages, Map *map);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);

};

/**
* PROJECTILE PAINT BALL
**/
class ProjectilePaintball : public Projectile {

    // Méthode redéfinie pour l'effet spécial
    void advance(int phase);

public:

    // Caractéristiques
    static const float DEFAULT_SPEED = 3;
      static const int DEFAULT_TARGET_TYPE = TYPE::T_RAMPANT | TYPE::T_VOLANT;


    ProjectilePaintball(float posX,float posY,float targetX,float targetY,int damages, Map *map);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);

};

/**
* PROJECTILE BOWLING
**/
class ProjectileBowling : public Projectile {

    // Méthode redéfinie pour l'effet spécial
    void advance(int phase);

public:

    // Caractéristiques
    static const float DEFAULT_SPEED = 2;
    static const int DEFAULT_TARGET_TYPE = TYPE::T_RAMPANT | TYPE::T_VOLANT;


    ProjectileBowling(float posX,float posY,float targetX,float targetY,int damages, Map *map);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);

};

#endif // PROJECTILE_H
