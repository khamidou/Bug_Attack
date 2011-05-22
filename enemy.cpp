#include <iostream>
#include "enemy.h"

/**
* ENEMY
**/

Enemy::Enemy(Map* map,int posx,int posy, float size):Entity(posx,posy),_map(map),_size(size),_destroyed(false){
    // Positionne l'ennemi bien au centre du chemin
    this->setPos(this->x() - (_size-1)*16,this->y() - (_size-1)*16);
    // Repertorie cet ennemi depuis la map
    _map->addEnemy(this);
}


int Enemy::getHP(void) const { return _hp; }
int Enemy::getResistance(void) const { return _resistance; }
bool Enemy::isDestroyed(void) const { return _destroyed; }

float Enemy::getSize(void) const { return _size; }
void Enemy::hurt(int damages){

    // Inflige 'damages' dégats à l'ennemi
    _hp-=damages;
    // Si ses HP sont tombés à zéro, on le détruit
    if(_hp <= 0) {
        // Indique que la mort est provoquée par le joueur
        emit killedByPlayer(this->getResistance()); // TODO choisir le gain

        // Retire de l'indexation de la map
        _destroyed = true;
        emit enemyDestroyed();
    }

    std::cout << "ahah il me reste : " << _hp << " HP" << std::endl;

}


/**
* CAFARD
**/


Cafard::Cafard(Map* map,int posx,int posy, float size):Enemy(map,posx,posy,size)
{
    // Données du cafard
    _type = TYPE::T_RAMPANT;
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
    Tile& currentTile = _map->getTileAt((this->x() + _size*16)/32,(this->y() + _size*16)/32);
    QPointF dirVect = currentTile.getDirection();

    float new_x = this->x() + _speed*dirVect.x();
    float new_y = this->y() + _speed*dirVect.y();

    this->setPos(new_x,new_y);

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
