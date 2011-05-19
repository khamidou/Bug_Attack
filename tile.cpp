#include "tile.h"
#include <iostream>
using namespace std;
using namespace TYPE;

Tile::Tile(int tileNumber, int x, int y):_tileNumber(tileNumber)
{
    // Direction associée au tile
    int  direction_bit_mask = tileNumber % 16; // Modulo 16 en cas de tile de départ

    // Déplacement horizontal
    // Ouest
    if((direction_bit_mask & WEST) == WEST)
        _dir.setX(-1);
    // Est
    if((direction_bit_mask & EAST) == EAST)
        _dir.setX(1);
    // Déplacement vertical
    // Nord
    if((direction_bit_mask & NORTH) == NORTH)
        _dir.setY(-1);
    // Sud
    if((direction_bit_mask & SOUTH) == SOUTH)
        _dir.setY(1);


    // DEBUG std::cout << "at " << x << "," << y << " ,tileNumber : " << tileNumber << ", dir.setX: " << _dir.x() << ", dir.setY: " << _dir.y() << std::endl;

    // Positionne relativement à la scène
    this->setPos(x*32,y*32);


    /* QString filePath("./media/textures/");
    filePath+= TexName(tileNumber); // TEST
    filePath+= QString(".jpg");
    cout << filePath.toStdString() << endl;*/
    _pixmap = new QPixmap("media/textures/plain/" + TexName(tileNumber) + ".png");
    if (_pixmap->isNull())
    cout << "pixmap null" << endl;

    _x = x;
    _y = y;
    *_pixmap = _pixmap->scaled(32,32,Qt::IgnoreAspectRatio);
    this->setPixmap(*_pixmap);

}

int Tile::getX() const { return _x; }
int Tile::getY() const { return _y; }
QVector2D Tile::getDirection(void) const { return _dir; }

bool Tile::turretAllowed(void) const {

    if(_tileNumber == 0)
        return true;
    return false;
}

QString Tile::TexName(int n){

    // Normal textures
    if((n & (TYPE::NORTH | TYPE::EAST | TYPE::UP)) == (TYPE::NORTH | TYPE::EAST | TYPE::UP))
        return "nw";
    if((n & (TYPE::NORTH | TYPE::EAST)) == (TYPE::NORTH | TYPE::EAST))
        return "se";
    if((n & (TYPE::NORTH | TYPE::WEST | TYPE::UP)) == (TYPE::NORTH | TYPE::WEST | TYPE::UP))
        return "ne";
    if((n & (TYPE::NORTH | TYPE::WEST)) == (TYPE::NORTH | TYPE::WEST))
        return "sw";
    if((n & (TYPE::SOUTH | TYPE::EAST | TYPE::UP)) == (TYPE::SOUTH | TYPE::EAST | TYPE::UP))
        return "ne";
    if((n & (TYPE::SOUTH | TYPE::EAST)) == (TYPE::SOUTH | TYPE::EAST))
        return "sw";
    if((n & (TYPE::SOUTH | TYPE::WEST | TYPE::UP)) == (TYPE::SOUTH | TYPE::WEST | TYPE::UP))
        return "nw";
    if((n & (TYPE::SOUTH | TYPE::WEST)) == (TYPE::SOUTH | TYPE::WEST))
        return "se";

    if( ((n & TYPE::NORTH) == TYPE::NORTH) ||
        ((n & TYPE::SOUTH) == TYPE::SOUTH)
      )
        return "vpath";

    if( ((n & TYPE::EAST) == TYPE::EAST) ||
        ((n & TYPE::WEST) == TYPE::WEST)
      )
        return "hpath";

    // Special textures
    if(n==32)
        return "goal";
    if(n==64)
        return "mud";

    return "grass";
}

Tile::~Tile(){}
