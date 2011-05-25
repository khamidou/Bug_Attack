#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "types.h"

class Player : public QObject
{
    Q_OBJECT

    int _money;
    int _lives;
    TYPE::TURRET _turretChoice;

public:

    static const int DEFAULT_MONEY = 100;
    static const int DEFAULT_LIVES = 10;

    Player(int money = DEFAULT_MONEY,int lives = DEFAULT_LIVES);
    bool payMoney(int amount);
    int getMoney(void) const;
    int getLives(void) const;
    TYPE::TURRET getTurretChoice(void) const;
    void setTurretChoice(TYPE::TURRET choice);


public slots:

    void earnMoney(int amount);
    void loseLives(int livesLost);
    void setTurretChoice1();

signals:
    void moneyValueChanged(int);
    void livesValueChanged(int);
};

#endif // PLAYER_H
