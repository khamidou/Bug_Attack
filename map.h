#ifndef MAP_H
#define MAP_H

#include <QFile>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>

#include "player.h"
#include "enemyfactory.h"
#include "tile.h"
#include "enemy.h"
#include "defenser.h"

class EnemyFactory;
class Enemy;
class Defenser;

class Map : public QGraphicsScene
{

    Q_OBJECT

    EnemyFactory* _waveGenerator;
    Tile* _mapTiles[16][16];
    QList<Enemy*> _enemies; // OPTIMISATION DES RECHERCHES D'ENNEMI
    QList<Defenser*> _turrets;
    // Defenser* _turrets[16][16];     OPTIMISATION RECHERCHE DE TOURELLE
    // Defenser* _selectedTurret;
    QGraphicsScene* _scene;
    QPoint _startPos;
    Player* _player;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);


public:
    Map(QWidget* parent,Player* player);
    void addEnemy(Enemy* enemy);
    bool addTurretAt(TYPE::TURRET turretType,float mx,float my);
    QPoint getStart(void) const;
    Tile& getTileAt(int grid_x, int grid_y) const;
    Defenser* getTurretAt(int grid_x,int grid_y);
    EnemyFactory* getWaveGenerator(void) const;
    QList<Enemy*> getEnemyList(void) const;


public slots:
    void removeEnemy(Enemy* ptr);
    void removeTurret(void);
    void upgradeTurret(void);

signals :
    void turretInfosRequest(QString);
    void setTurretsSelected(bool);
    void turretSold(int);
    void disableTurretUpgradeButton(bool);
    void disableTurretSellButton(bool);
};

#endif // MAP_H
