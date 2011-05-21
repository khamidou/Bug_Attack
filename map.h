#ifndef MAP_H
#define MAP_H

#include <QFile>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>

#include "enemyfactory.h"
#include "tile.h"
#include "entity.h"
#include "defenser.h"

class EnemyFactory;

class Map : public QGraphicsScene
{

private:

    // QList<int>* _ennemyList;
    EnemyFactory* _waveGenerator;
    Tile* _mapTiles[16][16];
    QList<Entity*> _entities;
    QGraphicsScene* _scene;
    QPoint _startPos;



public:
    Map(QObject* parent);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    QPoint getStart(void) const;
    Tile& getTileAt(int x, int y) const;
    EnemyFactory* getWaveGenerator(void) const;
};

#endif // MAP_H
