#include "player.h"


Player::Player(int money,int lives){

    // Initialise l'argent
    (money>0) ? _money = money : _money = DEFAULT_MONEY;
    // Intialise le nombre de vies
    (lives>0) ? _lives = lives : _lives = DEFAULT_LIVES;

}

int Player::getMoney(void) { return _money; }

void Player::earnMoney(int amount) {

    // Si on gagne (réellement) de l'argent, celui ci est ajouté au pactole
    // ... et le compteur est notifié
    if(amount >= 0) {
        _money+=amount;
        emit moneyValueChanged(_money);
    }
}

bool Player::payMoney(int amount) {

    // Vérifie si le joueur peut payer
    if(_money - amount >= 0) {
        _money-=amount;
        emit moneyValueChanged(_money);
        return true; // Le paiement fut possible
    }
    else
        return false; // Paiement impossible
}

