#include "player.h"
#include <iostream>

/** @brief Constructeur
    @param nombre d'unités d'argent au début de la party
    @param nombre de vies initial
    @param niveau de difficulté du jeu
*/

Player::Player(int money,int lives,GAME::DIFFICULTY level){

    // Initialise l'argent
    (money>0) ? _money = money : _money = DEFAULT_MONEY;
    // Intialise le nombre de vies
    (lives>0) ? _lives = lives : _lives = DEFAULT_LIVES;
    // Difficulté du jeu
    _gameDifficulty = level;
    // Choix clic
    _turretChoice = TYPE::NONE;

}

/** @brief accesseur; retourne le nombre d'unités d'argent que possède un joueur. */
int Player::getMoney(void) const { return _money; }

/** @brief accesseur; retourne le nombre de vies du joueur */
int Player::getLives(void) const { return _lives; }

/** @brief accesseur; retourne le niveau de difficulté de la partie courante*/
GAME::DIFFICULTY Player::getDifficulty(void) const{ return _gameDifficulty; }

/** @brief accesseur; ajuste le niveau de difficulté du jeu
    @param le niveau de difficulté du jeu
*/
void Player::setDifficulty(GAME::DIFFICULTY level) { _gameDifficulty = level; }

/** @brief accesseur; récupère le type de tourelle que le joueur a selectionné dans l'interface
    @return le type de la tourelle
*/
TYPE::TURRET Player::getTurretChoice(void) const { return _turretChoice; }

/** @brief augmente la quantité de monnaie que possède un joueur. Envoie un signal moneyValueChanged.
    @param la quantité de monnaie gagnée
*/
void Player::earnMoney(int money) {

    // Si on gagne (réellement) de l'argent, celui ci est ajouté au pactole
    // ... et le compteur est notifié
    if(money >= 0) {
        _money+=money;
        emit moneyValueChanged(_money);
    }
}

/** @brief dimininue le nombre de vies du joueur. Envoie un signal gameLost si le joueur a perdu.
    @param la quantité de monnaie gagnée
*/
void Player::loseLives(int livesLost) {

    // Fait perdre des vies au joueur
    // ... et le compteur est notifié
    _lives-=livesLost;
    if(_lives <= 0){
        _lives = 0;

        // Fin de partie : Perdu !
        emit gameLost(QString("Vous avez perdu..."));
    }

    emit livesValueChanged(_lives);
}

/** @brief augmente l'argent dont dispose le joueur après avoir vendu une tourelle. Envoie un signal moneyValueChanged.
    @param la quantité de monnaie gagnée
*/
void Player::turretSold(int money) {

    if(money>0)
       _money+=money;

    emit moneyValueChanged(_money);

}

/** @brief setter; fixe la tourelle choisie par le joueur dans l'interface. Envoie des signaux pour désactiver temporairement les boutons
                    correspondant à la tourelle choisie dans l'interface.
    @param le type de la tourelle
*/
void Player::setTurretChoice(TYPE::TURRET choice) {

    // Active le bouton associé à l'ancien choix
    if(choice != _turretChoice) {

        switch(_turretChoice) {

        case TYPE::PISTOLET_A_EAU :
            emit setTurret1ButtonDisabled(false);
            break;
        case TYPE::LANCE_PIERRES :
            emit setTurret2ButtonDisabled(false);
            break;
        case TYPE::PAINTBALL:
            emit setTurret3ButtonDisabled(false);
            break;
        case TYPE::PETANQUE:
            emit setTurret4ButtonDisabled(false);
            break;
        case TYPE::MUSICIEN:
            emit setTurret5ButtonDisabled(false);
            break;
        default:
            break;
        }
    }

    // Met à jour le choix courant
    _turretChoice = choice;

    // Désactive le bouton associé au nouveau choix
    if(_turretChoice != TYPE::NONE) {
        switch(_turretChoice) {
        case TYPE::PISTOLET_A_EAU:
            emit setTurret1ButtonDisabled(true);
            break;
        case TYPE::LANCE_PIERRES:
            emit setTurret2ButtonDisabled(true);
            break;
        case TYPE::PAINTBALL:
            emit setTurret3ButtonDisabled(true);
            break;
        case TYPE::PETANQUE:
            emit setTurret4ButtonDisabled(true);
            break;
        case TYPE::MUSICIEN:
            emit setTurret5ButtonDisabled(true);
            break;
        default:
            break;
        }
    }

}

void Player::setTurretChoice1(){this->setTurretChoice(TYPE::PISTOLET_A_EAU);}
void Player::setTurretChoice2(){this->setTurretChoice(TYPE::LANCE_PIERRES);}
void Player::setTurretChoice3(){this->setTurretChoice(TYPE::PAINTBALL);}
void Player::setTurretChoice4(){this->setTurretChoice(TYPE::PETANQUE);}
void Player::setTurretChoice5(){this->setTurretChoice(TYPE::MUSICIEN);}

/** @brief dimininue la quantité d'argent que possède un joueur. Envoie un signal moneyValueChanged.
    @param la quantité de monnaie depensée.
*/
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
