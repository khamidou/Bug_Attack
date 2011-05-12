#include "tile.h"
#include <iostream>
using namespace std;

Tile::Tile(int tileNumber)
{
    QString filePath("media/textures/");
    filePath+= TexName(tileNumber); // TEST
    filePath+= QString(".jpg");
    cout << filePath.toStdString() << endl;
    QPixmap texFile(filePath);
    texFile.scaled(32,32,Qt::IgnoreAspectRatio);
    this->setPixmap(texFile);

}

QString Tile::TexName(int n){

    // Normal textures
    if(n>0 && n<32){

        n%=16; // To keep a value between 0 and 10

        if(n==0)
            return "grass";
        if(n==1 || n==2)
            return "hpath";
        if(n==4 || n==8)
            return "vpath";
        if(n==5 || n==10)
            return "nepath";
        if(n==6 || n==9)
            return "nopath";


    }
    if(n==64)
        return "mud";
    // Special textures
    return "grass";
}

Tile::~Tile(){}
