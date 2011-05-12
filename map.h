#ifndef MAP_H
#define MAP_H

#include <QFile>
#include <QList>
#include <QGraphicsScene>
#include "tile.h"

class Map
{

private:

    // QList<int>* _ennemyList;
    Tile* _mapTiles[16][16];
    QGraphicsScene* _scene;


public:
    Map(QGraphicsScene* scene);
    ~Map();
    void LoadBackground(void);
};

#endif // MAP_H
