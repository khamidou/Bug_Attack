#ifndef PLAYER_H
#define PLAYER_H

#include <QMessageBox>
#include <QObject>
#include "types.h"

class Player : public QObject
{
    Q_OBJECT

    int _money;
    int _lives;
    GAME::DIFFICULTY _gameDifficulty;
    TYPE::TURRET _turretChoice;

public:

    static const int DEFAULT_MONEY = 100;
    static const int DEFAULT_LIVES = 10;

    Player(int money = DEFAULT_MONEY,int lives = DEFAULT_LIVES,GAME::DIFFICULTY = GAME::EASY);
    bool payMoney(int amount);
    int getMoney(void) const;
    int getLives(void) const;
    GAME::DIFFICULTY getDifficulty(void) const;
    TYPE::TURRET getTurretChoice(void) const;
    void setTurretChoice(TYPE::TURRET choice);
    void setDifficulty(GAME::DIFFICULTY level);


public slots:

    void earnMoney(int money);
    void loseLives(int livesLost);
    void turretSold(int money);
    void setTurretChoice1();
    void setTurretChoice2();
    void setTurretChoice3();
    void setTurretChoice4();
    void setTurretChoice5();


signals:
    void moneyValueChanged(int);
    void livesValueChanged(int);
    void setTurret1ButtonDisabled(bool);
    void setTurret2ButtonDisabled(bool);
    void setTurret3ButtonDisabled(bool);
    void setTurret4ButtonDisabled(bool);
    void setTurret5ButtonDisabled(bool);
    void turretSelected();
    void gameLost(QString);
};

#endif // PLAYER_H
