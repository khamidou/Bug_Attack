#include <iostream>
#include <fstream>
#include "map.h"



Map::Map(QObject *parent):QGraphicsScene(parent)
{

    // Chargement du contenu de la map
    std::ifstream fichier("media/maps/map.txt");
    int i, j;
    int num;
    if(fichier)
    {
        for(i = 0; i < 16; i++)
        {
            for(j = 0; j < 16; j++)
            {
                fichier >> num;
                _mapTiles[j][i] = new Tile(num, j, i);
                // Position de départ ?
                if(_mapTiles[j][i]->getTileNumber() == 18 )
                    _startPos = QPoint(32*j,32*i);
                // Ajoute le tile à la map courante
                this->addItem(_mapTiles[j][i]);
            }
        }
    }
    fichier.close();

    /* TEST
    _entities.push_front(new Cafard(this,32*2,0,1));
    this->addItem(_entities.first());
    */

    // Générateur de vagues
    _waveGenerator = new EnemyFactory(this);
}


// Gestion du clic de la souris
void Map::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    qreal mx = event->scenePos().x();
    qreal my = event->scenePos().y();

    if(event->button() == Qt::LeftButton &&
       this->getTileAt(mx/32,my/32).turretAllowed())
    {
        std::cout << "clic at : " << mx << "," <<  my << std::endl;
        _entities.push_front(new WaterGun(((int)(mx/32))*32,
                                          ((int)(my/32))*32,
                                          1,
                                          (TYPE::ENTITY)(TYPE::T_RAMPANT | TYPE::T_VOLANT))
                             );
        this->addItem(_entities.first());

    }
}

QPoint Map::getStart(void) const { return _startPos; }
Tile& Map::getTileAt(int x, int y) const { return *_mapTiles[x][y]; }
EnemyFactory* Map::getWaveGenerator(void) const { return _waveGenerator; }
