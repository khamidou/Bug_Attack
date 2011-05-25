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

    // Générateur de vagues
    _waveGenerator = new EnemyFactory(this);
}


// Gestion du clic de la souris
void Map::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    qreal mx = event->scenePos().x();
    qreal my = event->scenePos().y();


    // CLIC GAUCHE : POSE DE TOURELLE
    if(event->button() == Qt::LeftButton)
        this->addTurretAt(_player->getTurretChoice(),mx,my);

}


void Map::addEnemy(Enemy *enemy) {

    // Sauvegarde des ennemis sur la map
    _enemies.push_back(enemy);


    // En cas de destruction par le joueur, rapport de l'argent
    QObject::connect(enemy,SIGNAL(killedByPlayer(int)),_player,SLOT(earnMoney(int)));
    // Si l'ennemi atteint le goal, il est détruit et fait perdre une vie au joueur
    QObject::connect(enemy,SIGNAL(killedAtGoal(int)),_player,SLOT(loseLives(int)));
    // En cas de suppression de la mémoire, on doit aussi le désindexer depuis 'enemies'
    QObject::connect(enemy,SIGNAL(enemyDestroyed(Enemy*)),this,SLOT(removeEnemy(Enemy*)));

    // Ajout à la scène
    this->addItem(enemy);
}

void Map::removeEnemy(Enemy* ptr) {

    // Garbage collector de notre liste

    int i;
    for(i = 0; i < _enemies.length() ; ++i) {

        std::cout << _enemies[i] << " " << ptr << std::endl;
        if(_enemies[i] == ptr){
            std::cout << "OMG un mort" << std::endl;
            _enemies.removeAt(i);
           // QObject::disconnect(ptr);
            this->removeItem(ptr);
            break;
        }
    }
}

QList<Enemy*> Map::getEnemyList(void) const { return _enemies; };


bool Map::addTurretAt(TYPE::TURRET turretType,float mx,float my) {

    // Vérifie s'il est possible de poser une tourelle à cet emplacement
    if( this->getTileAt(mx/32,my/32).turretAllowed()) {

        // On pose une nouvelle tourelle du type souhaité ...
        // ... si le joueur a assez d'argent (le cas échant, il est débité aussitôt)
        Defenser* newTurret;
        bool enoughMoney = false;

        switch(turretType) {

            // Pistolet à eau
            case TYPE::PISTOLET_A_EAU:
            if(_player->payMoney(WaterGun::getBasicCost()))
            {
                enoughMoney = true;
                newTurret = new WaterGun(((int)(mx/32))*32,
                                         ((int)(my/32))*32,
                                         1,
                                         (TYPE::ENTITY)(TYPE::T_RAMPANT | TYPE::T_VOLANT),
                                         this);
            }
            break;

            // Type inconnu ~ erreur
            default:
            break;

         } // eos

        // Ajout de la tourelle et mise à jour des informations de la map
        if(enoughMoney) {
            // Le tile contient désormais une tourelle
            this->getTileAt(mx/32,my/32).setHasTurret(true);
            // Ajout sur la map
            this->addItem(newTurret);

            // Gestion des clics sur la tourelle
            // QObject::connect(newTurret,SIGNAL(turretClicked(QString)),this,SLOT(turretInfosRequest(QString)));

            // On remet à zéro le choix du joueur
            _player->setTurretChoice(TYPE::NONE);

            return true; // Succès
        }

    } // End test case libre

    return false; // Echec
}


QPoint Map::getStart(void) const { return _startPos; }
Tile& Map::getTileAt(int x, int y) const { return *_mapTiles[x][y]; }
EnemyFactory* Map::getWaveGenerator(void) const { return _waveGenerator; }
