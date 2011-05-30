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
    // Lancement du timer (boucle principale du jeu)
    QObject::connect(&gameTimer, SIGNAL(timeout()), this, SLOT(advance()));
    gameTimer.start(1000 / GAME::FPS);
}


// Gestion du clic de la souris
void Map::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    qreal mx = event->scenePos().x();
    qreal my = event->scenePos().y();

    // Par défaut déselectionne toute tourelle
    emit setTurretsSelected(false);
    emit disableTurretUpgradeButton(true);
    emit disableTurretSellButton(true);

    // CLIC GAUCHE
    if(event->button() == Qt::LeftButton) {
        // SELECTION D'UNE TOURELLE (affichage infos)
        if(Defenser* turret = this->getTurretAt((int)(mx/32)*32,(int)(my/32)*32)) {
            // Affichage des infos de la tourelle
            emit turretInfosRequest(turret->getInfos());
            turret->setIsSelected(true);
            // Si la tourelle n'est pas de niveau max, on autorise l'upgrade
            if(!turret->isLevelMax())
                emit disableTurretUpgradeButton(false);
            // On autorise la vente de la tourelle selectionnée
            emit disableTurretSellButton(false);

        }
        // POSE DE TOURELLE
        else{
            this->addTurretAt(_player->getTurretChoice(),mx,my);
        }
    }

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

        if(_enemies[i] == ptr) {
            std::cout << "OMG je suis mort" << std::endl;
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
                                         this);
            }
            break;

            // Lance-pierres
            case TYPE::LANCE_PIERRES:
            if(_player->payMoney(Slingshot::getBasicCost()))
            {
                enoughMoney = true;
                newTurret = new Slingshot(((int)(mx/32))*32,
                                         ((int)(my/32))*32,
                                         1,
                                         this);
            }
            break;

            // Paintball
            case TYPE::PAINTBALL:
            if(_player->payMoney(Paintball::getBasicCost()))
            {
                enoughMoney = true;
                newTurret = new Paintball(((int)(mx/32))*32,
                                         ((int)(my/32))*32,
                                         1,
                                         this);
            }
            break;

            // Pétanque
            case TYPE::PETANQUE:
            if(_player->payMoney(Bowling::getBasicCost()))
            {
                enoughMoney = true;
                newTurret = new Bowling(((int)(mx/32))*32,
                                         ((int)(my/32))*32,
                                         1,
                                         this);
            }
            break;

            // Type inconnu ~ erreur
            default:
            break;

         } // eos

        // Ajout de la tourelle et mise à jour des informations de la map
        if(enoughMoney) {
            // Ajout sur la map
            this->addItem(newTurret);
            // Sauvegarde dans la liste des tourelles
            _turrets.push_back(newTurret);
            // Gestion des clics sur la tourelle
            QObject::connect(this,SIGNAL(setTurretsSelected(bool)),newTurret,SLOT(setIsSelected(bool)));
              // On remet à zéro le choix du joueur
            _player->setTurretChoice(TYPE::NONE);

            return true; // Succès
        }

    } // End test case libre

    return false; // Echec
}

// Renvoie la tourelle positionnée à la position (x,y)
Defenser* Map::getTurretAt(int grid_x,int grid_y) {

    int i;
    for(i = 0; i < _turrets.length() ; ++i) {

         if(_turrets[i]->x() == grid_x && _turrets[i]->y() == grid_y)
            return _turrets[i];
    }
    // Si aucune tourelle trouvée
    return NULL;

}

void Map::removeTurret(void) {

    // Garbage collector de notre liste
    int i;
    for(i = 0; i < _turrets.length() ; ++i) {

         if(_turrets[i]->isSelected()){
             // Arrête tout tir en cours
             _turrets[i]->setIsShooting(false);
             // Donne de l'argent au joueur pour la vente
             emit turretSold(_turrets[i]->getCost()/2);
             // Retire l'objet de la map et de la liste
             this->removeItem(_turrets[i]);
             _turrets.removeAt(i);
             // Désactive les boutons de tourelle
             emit disableTurretUpgradeButton(true);
             emit disableTurretSellButton(true);
             // Met à jour l'affichage
             this->update();

            break;
        }
    }

}

void Map::upgradeTurret(void) {

    // Recherche la tourelle actuellement selectionnée et tente de l'améliorer

    int i;
    for(i = 0; i < _turrets.length() ; ++i) {

        if(_turrets[i]->isSelected())
        {
            // La tourelle peut être améliorée
            if(_turrets[i]->isLevelMax())
                break;

            // ... et si le joueur a assez d'argent
            if(_player->payMoney(_turrets[i]->getCost(_turrets[i]->getLevel()+1))) {
                // Level-up de la tourelle
                _turrets[i]->increaseLevel();
                // Mise à jour de la description
                emit turretInfosRequest(_turrets[i]->getInfos());

                // Si l'upgrade est maximale, on désactive le bouton d'upgrade
                if(_turrets[i]->isLevelMax())
                    emit disableTurretUpgradeButton(true);

                // Met à jour l'affichage
                this->update();
            }
            break;
        }
    }
}



QPoint Map::getStart(void) const { return _startPos; }
Tile& Map::getTileAt(int grid_x, int grid_y) const { return *_mapTiles[grid_x][grid_y]; }
EnemyFactory* Map::getWaveGenerator(void) const { return _waveGenerator; }

void Map::setPause(void) {
    if (_isPause) {
        _isPause = false;
        gameTimer.stop();
    } else {
        _isPause = true;
        gameTimer.start(1000 / GAME::FPS);
    }
}
