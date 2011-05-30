#include <iostream>
#include <math.h>
#include "enemy.h"

/**
* ENEMY
**/

Enemy::Enemy(Map* map,int posx,int posy, float size):Entity(posx,posy),_map(map),_size(size){
    // Positionne l'ennemi bien au centre du chemin
    this->setPos(this->x() - (_size-1)*16,this->y() - (_size-1)*16);
    // Désactive tout malus
    _malusCounter = 0;
    // Repertorie cet ennemi depuis la map
    _map->addEnemy(this);

}


int Enemy::getHP(void) const { return _hp; }
int Enemy::getResistance(void) const { return _resistance; }
TYPE::ENTITY Enemy::getType(void) const { return _type; }

void Enemy::setSpeedMalus(float percent, int seconds) {
    _malusCounter = GAME::FPS * seconds;
    if(percent >= 0 && percent <= 1)
        _speedMalus = percent; // Applique un pourcentage réducteur
    else
        _speedMalus = 0; // Par défaut on arrête l'ennemi
}


float Enemy::getSize(void) const { return _size; }
void Enemy::hurt(int damages){

    // Inflige 'damages' dégats à l'ennemi
    _hp-=damages;
    // Si ses HP sont tombés à zéro, on le détruit
    if(_hp <= 0) {
        // Indique que la mort est provoquée par le joueur
        emit killedByPlayer(this->getSize()); // TODO choisir le gain
        // Retire de l'indexation de la map
        emit enemyDestroyed(this);
        return;
    }

    std::cout << "ahah il me reste : " << _hp << " HP" << std::endl;

}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget*) {

    // Affiche l'image
    painter->drawPixmap(0,0,this->pixmap());

    // Affiche le conteneur de la barre de vie
    painter->setBrush(Qt::red);
    painter->drawRect(0,0,_size*16,5);

    // Affiche la vie courante
    painter->setBrush(Qt::green);
    painter->drawRect(0,0,(_hp*_size*16/_hpMax),5);

}

QRectF Enemy::boundingRect(void) const {
    return QRectF(0,0,_size*32,_size*32);
}

void Enemy::advance(int phase) {

    // Si 'phase' vaut 0, rien ne se passe
    if(!phase)
        return;
    // ...sinon, on met à jour l'item

    // Met à jour l'image de l'animation
    this->setPixmap(*_animPixmap[_animState]);
    this->increaseAnimationStep();



    // Déplace l'ennemi selon sa vitesse et la direction induite par le terrain
    Tile& currentTile = _map->getTileAt((this->x() + _size*16)/32,(this->y() + _size*16)/32);



    // L'ennemi est arrivé au "goal", on doit le détruire
    if(currentTile.getTileNumber() == TYPE::GOAL) {
        this->reachGoal();
        return;
    }



    /// Déplacement
    // Calcul d'éventuels malus
    float speedMalus = 1;  // Si aucun malus, cette valeur restera à 1, soit aucune modification
    if(_malusCounter > 0) {
        _malusCounter--;
        speedMalus = _speedMalus;
    }

    // Recherche de la direction à suivre
    QPointF dirVect = currentTile.getDirection();

    // Application de la vitesse
    float new_x = this->x() + _speed*dirVect.x()*speedMalus;
    float new_y = this->y() + _speed*dirVect.y()*speedMalus;

    this->setPos(new_x,new_y);

    // Oriente l'image dans le bon sens
    // (nb : transformation par rapport au centre de l'image)
    this->setTransformOriginPoint(this->boundingRect().center().x(),this->boundingRect().center().y());

    // Rotation
    if(dirVect.x() == 1 && dirVect.y() == 1)
        this->setRotation(45);
    else if(dirVect.x() == 1 && dirVect.y() == -1)
        this->setRotation(-45);
    else if(dirVect.x() == -1 && dirVect.y() == 1)
        this->setRotation(135);
    else if(dirVect.x() == -1 && dirVect.y() == -1)
        this->setRotation(-135);
    else if(dirVect.x() == 1)
        this->setRotation(0);
    else if(dirVect.x() == -1)
        this->setRotation(180);
    else if(dirVect.y() == 1)
        this->setRotation(90);
    else if(dirVect.y() == -1)
        this->setRotation(-90);

}


void Enemy::reachGoal(void) {

    // Indique que la mort est provoquée par l'atteinte du goal
    emit killedAtGoal(1); // Par défaut 1 vie perdue
    // Retire de l'indexation de la map
    emit enemyDestroyed(this);

}


/**
* CAFARD
**/


Cafard::Cafard(Map* map,int posx,int posy, float size):Enemy(map,posx,posy,size)
{
    // Données du cafard
    _type = DEFAULT_TYPE;
    _hp = 10*size*size;
    _hpMax = _hp;
    _resistance = 5*size*size;
    _speed = 2;

    // Images et animations
    for(int i = 0; i < 3; ++i) {
        QPixmap* animTemp = new QPixmap("./media/textures/ennemies/cafard" + QString::number(i+1) + ".png");
        *animTemp = animTemp->scaled(size*32,size*32);
        _animPixmap.push_back(animTemp);
    }
    _animState = 0; // Première frame de l'animation à 0

    // Image et taille
    this->setPixmap(*_animPixmap.first());
}

void Cafard::hurt(int damages) {
    Enemy::hurt(damages);
    if(_hp <= 0 && _size >= 2) {
        // A la mort d'un cafard, on créé deux cafards de taille/2
        Cafard *temp;
        temp = new Cafard(_map, this->x()+ (_size-1)*16,this->y()+ (_size-1)*16, _size / 2);
        // ... décale le cafard de quelques pixels
        for(int i = 0 ; i < 8 ; ++i) temp->advance(1);
        temp = new Cafard(_map,this->x()+ (_size-1)*16,this->y()+ (_size-1)*16, _size / 2);
    }

}

void Cafard::advance(int phase) {
    Enemy::advance(phase);
}



/**
* FOURMI
**/

Fourmi::Fourmi(Map* map,int posx,int posy, float size):Enemy(map,posx,posy,size)
{
    // Données de la fourmi
    _type = DEFAULT_TYPE;
    _hp = 5*size*size;
    _hpMax = _hp;
    _resistance = size*size;
    _speed = 2 + size/2;

    // Images et animations
    for(int i = 0; i < 3; ++i) {
        QPixmap* animTemp = new QPixmap("./media/textures/ennemies/fourmi" + QString::number(i+1) + ".png");
        *animTemp = animTemp->scaled(size*32,size*32);
        _animPixmap.push_back(animTemp);
    }
    _animState = 0; // Première frame de l'animation à 0

    // Image et taille
    this->setPixmap(*_animPixmap.first());
}

void Fourmi::hurt(int damages) {
    Enemy::hurt(damages);

    // Lorsqu'une fourmi est touchée, sa vitesse est multipliée par 1.5
    // ... pendant les 5 secondes suivant le dernier coup reçu
    if(!_speedUp)
        _speed*=1.5;
    _speedUp = true;
    _speedUpCounter = GAME::FPS * 5; // FPS * nbrSecondes voulues


}

void Fourmi::advance(int phase) {
    Enemy::advance(phase);

    // Si la fourmi est en mode accélération, on décrémente le compteur
    if(_speedUp) {

        _speedUpCounter--;

        // Si le compte à rebourd est terminé
        if(_speedUpCounter <= 0) {
            _speed /= 1.5;
            _speedUp = false;
        }
    }
}



/**
* GUEPE
**/

Guepe::Guepe(Map* map,int posx,int posy, float size):Enemy(map,posx,posy,size)
{
    // Données de la guepe
    _type = DEFAULT_TYPE;
    _hp = 7*size*size;
    _hpMax = _hp;
    _resistance = 4*size*size;
    _speed = 3;

    // Images et animations
    for(int i = 0; i < 2; ++i) {
        QPixmap* animTemp = new QPixmap("./media/textures/ennemies/guepe" + QString::number(i+1) + ".png");
        *animTemp = animTemp->scaled(size*32,size*32);
        _animPixmap.push_back(animTemp);
    }
    _animState = 0; // Première frame de l'animation à 0

    // Image et taille
    this->setPixmap(*_animPixmap.first());
}

void Guepe::hurt(int damages) {
    Enemy::hurt(damages);

    // Si l'abeille meurt, elle s'écrase au sol et inflige 5*taille² dégâts
    // ... aux ennemis à moins d'une case et demi de distance
    if(_hp <= 0) {

        QList<Enemy*> enemies = _map->getEnemyList();

        QList<Enemy*>::iterator i;

        // Recherche des ennemis de la map
        for(i = enemies.begin() ; i != enemies.end() ; ++i) {

            if((*i)->getType() == TYPE::T_RAMPANT) {

                float targetX = (*i)->x();
                float targetY = (*i)->y();

                // arrête case = 32 pixel ==> demi case = 16 pixel
                if(sqrt(pow(fabs(this->x()-16 - targetX - ((*i)->getSize()-1)*16),2)
                        +pow(fabs(this->y()-16 - targetY - ((*i)->getSize()-1)*16),2)
                    )  <= 16)
                {
                   // On inflige des dommages aux enemis rampants
                   (*i)->hurt(_size*_size*5);
                   // std::cout << "bim une guepe s'est ecrasee" << std::endl;

                } // end distanceTest
            } // end test T_RAMPANT

        } //eof
    } // fin si hp <= 0

}

void Guepe::advance(int phase) {
    Enemy::advance(phase);
}


/**
* MOUSTIQUE
**/

Moustique::Moustique(Map* map,int posx,int posy, float size):Enemy(map,posx,posy,size)
{
    // Données du moustique
    _type = DEFAULT_TYPE;
    _hp = 6*size*size;
    _hpMax = _hp;

    this->updateStats();

    // Images et animations
    for(int i = 0; i < 2; ++i) {
        QPixmap* animTemp = new QPixmap("./media/textures/ennemies/moustique" + QString::number(i+1) + ".png");
        *animTemp = animTemp->scaled(size*32,size*32);
        _animPixmap.push_back(animTemp);
    }
    _animState = 0; // Première frame de l'animation à 0

    // Image et taille
    this->setPixmap(*_animPixmap.first());
}

void Moustique::updateStats(void) {

    // Les caractéristiques du moustique dépendent de son état actuel
    // (ie volant ou rampant)

    // En vol
    if(_type == TYPE::T_VOLANT) {
        _resistance = _size*_size;
        _speed = 2 + _size/2;
    }
    // Au sol
    else {
        _resistance = 15*_size;
        _speed = 1 + _size/2;
    }

}

void Moustique::hurt(int damages) {
    Enemy::hurt(damages);

    // Si le moustique est touché, il se pose pendant 3 secondes
    if(_type != TYPE::T_RAMPANT) {
        _type = TYPE::T_RAMPANT;
        this->updateStats();
    }

    // Reste au sol 3 secondes
    _stateCounter = GAME::FPS * 3;
}

void Moustique::advance(int phase) {
    Enemy::advance(phase);

    // Décrémente le compteur
    _stateCounter--;

    // Changement d'état
    if(_stateCounter <= 0) {

        // Le moustique alterne 7 secondes en vol
        if(_type == TYPE::T_RAMPANT) {

            _type = TYPE::T_VOLANT;
            this->updateStats();
            _stateCounter = GAME::FPS * 7;

        }
        // ... et 3 secondes au sol
        else {

            _type = TYPE::T_RAMPANT;
            this->updateStats();
            _stateCounter = GAME::FPS * 3;

        }
    } // end changement d'état

}


