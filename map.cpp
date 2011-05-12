#include <iostream>
#include <fstream>
#include "map.h"



Map::Map(QGraphicsScene* scene)
{

    // Container
    _scene = scene;

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
                std::cout << num << std::endl;
                _mapTiles[i][j] = new Tile(num);
            }
        }
    }
}

void Map::LoadBackground(void) {

    for(int x = 0 ; x < 16 ; ++x){
        for(int y = 0 ; y < 16 ; ++y){
            _mapTiles[x][y]->setPos(y*32,x*32);
            _scene->addItem(_mapTiles[x][y]);
        }
    }
}

Map::~Map(){


}
