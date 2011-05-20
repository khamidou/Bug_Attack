#include "cafard.h"
#include <iostream>
using namespace TYPE;

Cafard::Cafard(Map* map, int posx,int posy, float size):Monster(posx,posy,size),_map(map)
{
    // Données du cafard
    _type = T_RAMPANT;
    _hp = 10*size*size;
    _resistance = 5*size*size;
    _speed = 2;

    // Images et animations
    for(int i = 0; i < 3; ++i) {
        QPixmap* animTemp = new QPixmap("./media/textures/ennemies/cafard" + QString::number(i+1) + ".png");
        *animTemp = animTemp->scaled(size*32,size*32);
        _animPixmap.push_back(animTemp);
    }
    _animState = 0; // Première frame de l'animation à 0

    // Image et taille
    this->setPixmap(*_animPixmap.first());

    // Position
    this->setPos(_x,_y);
}

void Cafard::advance(int phase) {

    // Si 'phase' vaut 0, rien ne se passe
    if(!phase)
        return;
    // ...sinon, on met à jour l'item

    // Met à jour l'image de l'animation
    this->setPixmap(*_animPixmap[_animState]);
    this->increaseAnimationStep();

    // Déplace l'ennemi selon sa vitesse et la direction induite par le terrain
    Tile& currentTile = _map->getTileAt((_x+16)/32,(_y+16)/32);
    QVector2D dirVect = currentTile.getDirection();

    _x += _speed*dirVect.x();
    _y += _speed*dirVect.y();

    this->setPos(_x,_y);

    // Oriente l'image dans le bon sens
    // (nb : transformation par rapport au centre de l'image)
    this->setTransformOriginPoint(this->boundingRect().center().x(),this->boundingRect().center().y());

    // Rotation
    if(dirVect.x() == 1 && dirVect.y() == 1)
        this->setRotation(45);
    else if(dirVect.x() == 1 && dirVect.y() == -1)
        this->setRotation(-45);
    else if(dirVect.x() == -1 && dirVect.y() == 1)
        this->setRotation(135);
    else if(dirVect.x() == -1 && dirVect.y() == -1)
        this->setRotation(-135);
    else if(dirVect.x() == 1)
        this->setRotation(0);
    else if(dirVect.x() == -1)
        this->setRotation(180);
    else if(dirVect.y() == 1)
        this->setRotation(90);
    else if(dirVect.y() == -1)
        this->setRotation(-90);



}
