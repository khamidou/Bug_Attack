#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>

class Tile : public QGraphicsPixmapItem
{
public:

    Tile(int tileNumber);
    ~Tile(void);

    QString TexName(int n);


};

#endif // TILE_H
