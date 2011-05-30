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

void Player::earnMoney(int money) {

    // Si on gagne (réellement) de l'argent, celui ci est ajouté au pactole
    // ... et le compteur est notifié
    if(money >= 0) {
        _money+=money;
        emit moneyValueChanged(_money);
    }
}

void Player::loseLives(int livesLost) {

    // Fait perdre des vies au joueur
    // ... et le compteur est notifié
    _lives-=livesLost;
    if(_lives <= 0){
        _lives = 0;

        //!-- BLOC DE FIN DE PARTIE : Victoire
        QMessageBox msgBox;
        msgBox.setText("Fin de partie");
        msgBox.setInformativeText("Vous avez perdu...");
        QPushButton *restartButton = msgBox.addButton(tr("Rejouer"), QMessageBox::ActionRole);
        QPushButton *quitButton = msgBox.addButton(tr("Quitter"), QMessageBox::ActionRole);
        msgBox.exec();
        if (msgBox.clickedButton() == (QAbstractButton *) restartButton) {

        }
        else if (msgBox.clickedButton() == (QAbstractButton *) quitButton)
             exit(0);
        // FIN DU BLOC  --!
    }

    emit livesValueChanged(_lives);
}

void Player::turretSold(int money) {

    if(money>0)
       _money+=money;

    emit moneyValueChanged(_money);

}

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
        default:
            break;
        }
    }

}
void Player::setTurretChoice1(){this->setTurretChoice(TYPE::PISTOLET_A_EAU);}
void Player::setTurretChoice2(){this->setTurretChoice(TYPE::LANCE_PIERRES);}
void Player::setTurretChoice3(){this->setTurretChoice(TYPE::PAINTBALL);}
void Player::setTurretChoice4(){this->setTurretChoice(TYPE::PETANQUE);}



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
