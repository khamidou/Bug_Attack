#ifndef MAP_H
#define MAP_H

#include <QFile>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>

#include "tile.h"
#include "entity.h"
#include "cafard.h"
#include "watergun.h"


class Map : public QGraphicsScene
{

private:

    // QList<int>* _ennemyList;
    Tile* _mapTiles[16][16];
    QList<Entity*> _entities;
    QGraphicsScene* _scene;


public:
    Map(QObject* parent);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    Tile& getTileAt(int x, int y) const;
};

#endif // MAP_H
