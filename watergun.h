#ifndef WATERGUN_H
#define WATERGUN_H

#include <QPainter>
#include "defenser.h"
#include "map.h"

class Map;

class WaterGun : public Defenser
{
public:
    WaterGun(int posx,int posy,int level,TYPE::ENTITY target,Map* map);
    int getCost(void) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*);
    QRectF boundingRect() const;
    void shootAt(float targetX,float targetY);
protected:
    void advance(int phase);
    void updateStats(void);

    Map* _map;
};

#endif // WATERGUN_H
