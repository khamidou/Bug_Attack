#ifndef WATERGUN_H
#define WATERGUN_H

#include <QPainter>
#include "defenser.h"
#include "map.h"

class Map;

class WaterGun : public Defenser
{
public:
    WaterGun(int posx,int posy,int level,TYPE::ENTITY target);
    int getCost(void) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);
    QRectF boundingRect() const;
protected:
    void advance(int phase);
    void updateStats(void);

    Map* _map;
};

#endif // WATERGUN_H
