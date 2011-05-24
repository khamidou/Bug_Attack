#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player : public QObject
{
    Q_OBJECT

    int _money;
    int _lives;

public:

    static const int DEFAULT_MONEY = 100;
    static const int DEFAULT_LIVES = 10;

    Player(int money = DEFAULT_MONEY,int lives = DEFAULT_LIVES);
    bool payMoney(int amount);
    int getMoney(void) const;
    int getLives(void) const;

public slots:

    void earnMoney(int amount);
    void loseLives(int livesLost);

signals:
    void moneyValueChanged(int);
    void livesValueChanged(int);
};

#endif // PLAYER_H
