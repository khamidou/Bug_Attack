#include <QMenu>
#include <iostream>
#include <fstream>
#include "map.h"



Map::Map(QWidget *parent,Player* player):QGraphicsScene(parent),_player(player)
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


    // CLIC GAUCHE : POSE DE TOURELLE
    if(event->button() == Qt::LeftButton)
        this->addTurretAt(mx,my);

}


void Map::addEnemy(Enemy *enemy) {

    // Sauvegarde des ennemis sur la map
    _enemies.push_back(enemy);

    // En cas de destruction par le joueur, rapport de l'argent
    QObject::connect(enemy,SIGNAL(killedByPlayer(int)),_player,SLOT(earnMoney(int)));
    // En cas de suppression de la mémoire, on doit aussi le désindexer depuis 'enemies'
    QObject::connect(enemy,SIGNAL(enemyDestroyed()),this,SLOT(removeEnemy()));

    // Ajout à la scène
    this->addItem(enemy);
}

void Map::removeEnemy(void) {

    // Garbage collector de notre liste

    QList<Enemy*>::iterator i;
    for(i = _enemies.begin() ; i != _enemies.end() ; ++i) {

        if((*i)->isDestroyed()){
            this->removeItem((*i));
            _enemies.erase(i);
            std::cout << "OMG un mort" << std::endl;
        }
    }
}

QList<Enemy*> Map::getEnemyList(void) const { return _enemies; };


bool Map::addTurretAt(float mx,float my) {

    // Vérifie s'il est possible de poser une tourelle à cet emplacement
    // ... et si le joueur a assez d'argent (le cas échant, il est débité aussitôt
    if( this->getTileAt(mx/32,my/32).turretAllowed() &&
        _player->payMoney(WaterGun::getBasicCost())) {

        this->getTileAt(mx/32,my/32).setHasTurret(true);
        WaterGun* test = new WaterGun(((int)(mx/32))*32,
                                          ((int)(my/32))*32,
                                          1,
                                          (TYPE::ENTITY)(TYPE::T_RAMPANT | TYPE::T_VOLANT),
                                          this);
        this->addItem(test);

        return true; // Succès

    }

    return false; // Echec
}

QPoint Map::getStart(void) const { return _startPos; }
Tile& Map::getTileAt(int x, int y) const { return *_mapTiles[x][y]; }
EnemyFactory* Map::getWaveGenerator(void) const { return _waveGenerator; }
