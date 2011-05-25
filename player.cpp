#include "player.h"
#include <iostream>


Player::Player(int money,int lives){

    // Initialise l'argent
    (money>0) ? _money = money : _money = DEFAULT_MONEY;
    // Intialise le nombre de vies
    (lives>0) ? _lives = lives : _lives = DEFAULT_LIVES;

    // Choix clic
    _turretChoice = TYPE::NONE;

}

int Player::getMoney(void) const { return _money; }
int Player::getLives(void) const { return _lives; }
TYPE::TURRET Player::getTurretChoice(void) const { return _turretChoice; }

void Player::earnMoney(int amount) {

    // Si on gagne (réellement) de l'argent, celui ci est ajouté au pactole
    // ... et le compteur est notifié
    if(amount >= 0) {
        _money+=amount;
        emit moneyValueChanged(_money);
    }
}

void Player::loseLives(int livesLost) {

    // Fait perdre des vies au joueur
    // ... et le compteur est notifié
    _lives-=livesLost;
    if(_lives < 0)
        _lives = 0;

    emit livesValueChanged(_lives);
}

void Player::setTurretChoice(TYPE::TURRET choice) {
    _turretChoice = choice;
    std::cout << "Le joueur a choisi " << choice << std::endl;
}
void Player::setTurretChoice1(){this->setTurretChoice(TYPE::PISTOLET_A_EAU);}



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
