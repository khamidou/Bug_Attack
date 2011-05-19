#ifndef ENTITY_H
#define ENTITY_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QList>

#include "types.h"

class Entity : public QGraphicsPixmapItem // , public QObject
{

   // Q_OBJECT // Utilisé pour les signaux/slots

public:

    Entity(int posX, int posY);
    void increaseAnimationStep(void);
    int getX() const;
    int getY() const;



protected:

    virtual void advance(int phase) = 0;

    QList<QPixmap*> _animPixmap;
    int _x;
    int _y;
    int _animState; // Frame courante de l'animation
    TYPE::ENTITY _type;


};

#endif // ENTITY_H

