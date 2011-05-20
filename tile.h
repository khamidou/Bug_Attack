#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>
#include <QVector2D>
#include "types.h"

// Masque binaire pour déterminer l'orientation d'un tile

class Tile : public QGraphicsPixmapItem
{
public:

    Tile(int tileNumber, int x, int y);
    ~Tile(void);
    int getX() const;
    int getY() const;    
    int getTileNumber(void) const;
    bool turretAllowed(void) const;
    QVector2D getDirection(void) const;


    QString TexName(int n);

private:
    QPixmap *_pixmap;
    int _x, _y;
    int _tileNumber;
    QVector2D _dir;
};

#endif // TILE_H
