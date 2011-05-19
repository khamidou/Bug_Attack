#include <iostream>
#include <fstream>
#include "map.h"



Map::Map(QObject *parent):QGraphicsScene(parent)
{
    /*   // Loads map file
    QFile mapFile("media/maps/map.txt");
    if(!mapFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    // Reads map data and creates corresponding map tiles
    int y = 0;
    while(!mapFile.atEnd()){
        QByteArray line = mapFile.readLine();
        QList<QByteArray> tileNumber = line.split(' ');
        for(int x = 0; x < tileNumber.size(); ++x)
             _mapTiles[x][y] = new Tile(tileNumber[x].toInt());
        y++;
    }
    // Close file
    mapFile.close();*/

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
                this->addItem(_mapTiles[j][i]);
            }
        }
    }
    fichier.close();

    _entities.push_front(new Cafard(this,32*2,0,1));
    this->addItem(_entities.first());

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

Tile& Map::getTileAt(int x, int y) const { return *_mapTiles[x][y]; }
