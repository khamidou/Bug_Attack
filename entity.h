#ifndef ENTITY_H
#define ENTITY_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QList>

#include "types.h"

class Entity : public QGraphicsPixmapItem, public QObject
{

   // Q_OBJECT utilisé pour les signaux/slots

public:

    Entity(float posX, float posY);
    void increaseAnimationStep(void);



protected:

    virtual void advance(int phase) = 0;

    QList<QPixmap*> _animPixmap;
    int _animState; // Frame courante de l'animation
    TYPE::ENTITY _type;


};

#endif // ENTITY_H

