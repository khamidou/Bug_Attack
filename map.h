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

class Map : public QGraphicsScene
{

    Q_OBJECT

    // QList<int>* _ennemyList;
    EnemyFactory* _waveGenerator;
    Tile* _mapTiles[16][16];
    QList<Enemy*> _enemies; // OPTIMISATION DES RECHERCHES D'ENNEMI
    QGraphicsScene* _scene;
    QPoint _startPos;
    Player* _player;



public:
    Map(QWidget* parent,Player* player);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void addEnemy(Enemy* enemy);
    bool addTurretAt(float mx,float my);
    QPoint getStart(void) const;
    Tile& getTileAt(int x, int y) const;
    EnemyFactory* getWaveGenerator(void) const;
    QList<Enemy*> getEnemyList(void) const;

public slots:
    void removeEnemy(void);

signals:
    void enemyDestroyed(int amountEarned);

};

#endif // MAP_H
