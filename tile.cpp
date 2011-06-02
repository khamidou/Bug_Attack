#include "tile.h"
#include <iostream>
using namespace std;
using namespace TYPE;

/**
* @brief Constructeur
* @param le type (numéro) de la tile
* @param abscisse de la tile
* @param ordonnée de la tile
*/
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


    // Chargement image correspondante
    _pixmap = new QPixmap("media/textures/plain/" + TexName(tileNumber) + ".png");
    if (_pixmap->isNull())
    cout << "pixmap null" << endl;

    // Données du tile
    _x = x;
    _y = y;

    *_pixmap = _pixmap->scaled(32,32,Qt::IgnoreAspectRatio);
    this->setPixmap(*_pixmap);

}

/**
* @brief Accesseur de l'abscisse de la tile
*/
int Tile::getX() const { return _x; }

/**
* @brief Accesseur de l'ordonnée de la tile
*/
int Tile::getY() const { return _y; }

/**
* @brief Accesseur du type de la Tile
*/
int Tile::getTileNumber(void) const { return _tileNumber; }

/**
* @brief Renvoie un vecteur correspondant à la direction de la Tile.
*/
QPointF Tile::getDirection(void) const { return _dir; }

/**
* @brief Est-ce qu'une tourelle est autorisée à cet endroit ?
*/
bool Tile::turretAllowed(void) const {

    if(_tileNumber == 0)
        return true;
    return false;
}

/**
* @brief Associe un nom de tourelle à une direction.
*/
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
