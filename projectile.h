#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QPainter>
#include "entity.h"


class Projectile : public Entity
{

   float _speed;
   int _damages;
   float _targetX;
   float _targetY;
   QPointF _movementVect;

   virtual void advance(int phase);

public:
   QRectF boundingRect() const;
   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);

   Projectile(float posX,float posY,float targetX,float targetY,float speed,int damages);


};

#endif // PROJECTILE_H
