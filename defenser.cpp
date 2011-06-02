#include <math.h>
#include <iostream>
#include "defenser.h"
#include "projectile.h"

/************************************************************************************************************
* DEFENSER
************************************************************************************************************/

/**
* @brief Constructeur
* @param posx Position x du défenseur, doit être multiple de 32.
* @param posy Position y du défenseur, doit être multiple de 32.
* @param level Niveau de départ de la tourelle (entier positif).
* @param map Pointeur sur la map contenant le défenseur.
*/
Defenser::Defenser(int posx,int posy,unsigned int level,Map* map)
    :Entity(posx,posy),_level(level),_map(map)
{
    this->setIsSelected(true); // Appel à la méthode pour rafraichir l'affichage
    // Met le compteur interne à 0 et relie le FPS au tir
    _shootTimerStep = 0;
    _powerBonus = 1;
}

/**
* @brief Indique si le défenseur est selectionné par un clic ou non (effets graphiques).
* @param selected Vrai si selectionné.
*/
void Defenser::setIsSelected(bool selected) {
    _isSelected = selected;
    _map->update(); // Met à jour l'affichage
}

/**
* @brief Renvoie le niveau actuel du défenseur.
* @return _level Niveau du défenseur.
*/
unsigned int Defenser::getLevel(void) const { return _level; }


/**
* @brief Indique si la tourelle est actuellement selectionnée suite à un clic de souris.
* @return _isSelected Renvoie vrai si la tourelle a été selectionnée.
*/
bool Defenser::isSelected(void) const { return _isSelected; }

/**
* @brief Augmente de un niveau le défenseur.
*/
void Defenser::increaseLevel(void) {
    if(_level < 3) _level++;
    this->updateStats();
}


/**
* @brief Méthode propre au QGraphicsItem appelée à chaque itération du FPS.
* Recherche les éventuels ennemis et active le tir si possible.
*
* @param phase Cette méthode est appelée automatiquement avec phase = 1 lorsque une mise à jour doit être effectuée, 0 sinon.
*/
void Defenser::advance(int phase) {
    if(!phase)
        return;

    // Détection des ennemis aux alentours
    QList<Enemy*> enemies = _map->getEnemyList();

    QList<Enemy*>::iterator i;

    // Recherche des ennemis de la map (tire sur tous les types d'ennemis)
    for(i = enemies.begin() ; i != enemies.end() ; ++i) {

            float targetX = (*i)->x();
            float targetY = (*i)->y();

            if(    (_targetType | (*i)->getType()) == _targetType &&
                    sqrt(pow(fabs(this->x()-16 - targetX - ((*i)->getSize()-1)*16),2)
                    +pow(fabs(this->y()-16 - targetY - ((*i)->getSize()-1)*16),2)
                )  <= _range)
            {
                // Si le défenseur ne tirait pas encore
                if(!_isShooting) {
                    this->setIsShooting(true); // Modification via la méthode pour les effets visuels
                }
                // Actualise la position de la cible
                this->setTarget(targetX+(*i)->getSize()*16,targetY+(*i)->getSize()*16);
                // Tire
                this->shootTarget();

                return; // On vise la première cible rencontrée

            } // end distanceTest

    } //eof
    // Si aucun monstre n'a été rencontré
    this->setIsShooting(false);  // Modification via la méthode pour les effets visuels

} // eom


/**
* @brief Méthode propre au QGraphicsItem appelée lors des tests de collisions ou pour l'affichage.
* @return rect Bounding box du défenseur (ici le rectangle d'une case 32*32).
*/
QRectF Defenser::boundingRect(void) const {
    return QRectF(-32,-32,_range,_range);
}


/**
* @brief Méthode propre au QGraphicsItem appelée lorsqu'il est nécessaire de redessiner le défenseurs.
* @param painter Objet de dessin utilisé par la scène.
*/
void Defenser::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) {
    // Si la tourelle est selectionnée...
    if(_isSelected) {

        painter->save();

        // Dessine la portée de la tourelle
        painter->setOpacity(0.2f);

            // Si on a un bonus, la portée est dessinée en dorée
            if(_powerBonus > 1)
                painter->setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
            // Sinon on l'affiche en blanc
            else
                painter->setBrush(QBrush(Qt::white,Qt::SolidPattern));

        painter->drawEllipse((-_range/32)*16,(-_range/32)*16,_range+32,_range+32);

        painter->restore();
    }
}

/**
* @brief Sauvegarde la position actuelle de la cible du défenseur.
* @param targetX Position X de la cible.
* @param targetY Position Y de la cible.
*/
void Defenser::setTarget(float targetX,float targetY) {
    // Sauvegarde la position de la cible courante
    _targetX = targetX;
    _targetY = targetY;
}


/**
* @brief Active/Désactive le mode tir (effets visuels).
* @param state Vrai pour activer le mode tir.
*/
void Defenser::setIsShooting(bool state) {

    _isShooting = state;

    // MODE TIR ACTIVÉ
    if(state)
    {
        // Change l'aspect visuel
        this->setOpacity(0.5f); // TEST = TIR ACTIVÉ
        // Active le timer
        //_shootTimer.start(100*_rate);
    }
    // MODE TIR DESACTIVÉ
    else
    {
        // Change l'aspect visuel
        this->setOpacity(1.0f); // TEST = TIR ACTIVÉ
        // Désactive le timer
        //_shootTimer.stop();
    }
}

/**
* @brief Indique si le défenseur est actuellement en train de tirer ou non.
* @return state Vrai si le défenseur tire.
*/
bool Defenser::isShooting(void) const { return _isShooting;}


/**
* @brief Indique si le défenseur est actuellement au niveau max.
* On considère que par défaut seules trois améliorations sont possibles.
* Cette méthode est néanmoins virtuelle pour cas particulier(s).
*
* @return isLevelmax Vrai si le niveau max a été atteind.
*/
bool Defenser::isLevelMax(void) const { return _level==3; }


/**
* @brief Applique un bonus d'attaque au défenseur.
* Si un bonus existe déjà, le bonus le plus fort est gardé.
*
* @param powerBonus pourcentage d'amélioration du tir (>=1).
*/
void Defenser::setBonus(float powerBonus) {

    // Si un bonus existe déjà, le bonus le plus fort est gardé
    if(_powerBonus > powerBonus)
        return;
    _powerBonus = powerBonus;

}


/**
* @brief Retire l'éventuel bonus d'attaque de la tourelle (i.e remet le bonus à 1).
*/
void Defenser::removeBonus(void) {
    _powerBonus = 1;
}



/************************************************************************************************************
* PISTOLET A EAU
************************************************************************************************************/

/**
* @brief Constructeur
* @param posx Position x du défenseur, doit être multiple de 32.
* @param posy Position y du défenseur, doit être multiple de 32.
* @param level Niveau de départ de la tourelle (entier positif).
* @param map Pointeur sur la map contenant le défenseur.
*/
WaterGun::WaterGun(int posx,int posy,unsigned int level,Map* map)
    : Defenser(posx,posy,level,map)
{
    // Type de cible
    _targetType = DEFAULT_TARGET_TYPE;
    // Caractéristiques
    this->updateStats();
}


/**
* @brief Renvoie le coût nécessaire à l'achat/amélioration
* @param level Niveau dont on souhaite avoir le prix (si 0 donné en paramètre, renvoie le coût du niveau actuel).
* @return cost Prix de l'achat/amélioration.
*/
int WaterGun::getCost(unsigned int level) const{

    // Si aucune valeur en argument, alors on affiche le coût du niveau courant
    if(level == 0) level = _level;

    switch(level) {
        case 1: return BASIC_COST;
        case 2: return INTERMEDIATE_COST;
        default: return HIGHEST_COST;
    }
}


/**
* @brief Renvoie un texte contenant des informations sur le défenseur (niveau, puissance, amélioration etc...).
* @return infos Texte contenant les informations.
*/
QString WaterGun::getInfos(void) const {

    // Infos générales
    QString infos = "Pistolet a eau de niveau ";
    infos+= QString::number(_level);
    // Amélioration
    if(!this->isLevelMax()) {
        infos+= "\n- Amelioration : ";
        infos+= QString::number(this->getCost(_level+1));
    }
    // Revente
    infos+= "\n- Revente : ";
    infos+= QString::number(this->getCost(_level)/2);

    return infos;
}

/**
* @brief Met à jours les caractéristiques du défenseur, en se basant sur son niveau actuel.
*/
void WaterGun::updateStats(void) {

    _range = (2.0f + _level/2.0f)*32.0f;
    _rate = 4 - _level/2;
    _power = 5 * _level * sqrt(_level); // level ^ 1.5

}

/**
* @brief Méthode propre au QGraphicsItem appelée lorsqu'il est nécessaire de redessiner le défenseur.
* @param painter Objet de dessin utilisé par la scène.
*/
void WaterGun::paint(QPainter *painter, const QStyleOptionGraphicsItem *so, QWidget *w) {

    // Dessine la portée de la tourelle si celle ci est selectionnée
    Defenser::paint(painter,so,w);

    // Dessine la tourelle
    painter->setBrush(QBrush(Qt::blue));
    painter->drawRect(0,0,32,32);

}

/**
* @brief Tire un projectile sur la cible actuellement enregistrée.
* Le tir est effectué selon le compteur interne de fréquence de tir.
*/
void WaterGun::shootTarget(void) {
    if (!isShooting())
        return;

    _shootTimerStep+= 1;

    if (_shootTimerStep < (GAME::FPS / _rate))
        return;

    _shootTimerStep = 0;

    // Créé un nouveau projectile au niveau du centre de la tourelle
    Projectile* shot = new ProjectileWater(this->x()+14,this->y()+14,_targetX,_targetY,_power*_powerBonus, _map);
    _map->addItem(shot);

}



/************************************************************************************************************
* LANCE-PIERRES
************************************************************************************************************/

/**
* @brief Constructeur
* @param posx Position x du défenseur, doit être multiple de 32.
* @param posy Position y du défenseur, doit être multiple de 32.
* @param level Niveau de départ de la tourelle (entier positif)
* @param map Pointeur sur la map contenant le défenseur
*/
Slingshot::Slingshot(int posx,int posy,unsigned int level,Map* map)
    : Defenser(posx,posy,level,map)
{
    // Type de cible
    _targetType = DEFAULT_TARGET_TYPE;
    _shootTimerStep = 0;
    // Caractéristiques
    this->updateStats();
}


/**
* @brief Renvoie le coût nécessaire à l'achat/amélioration
* @param level Niveau dont on souhaite avoir le prix (si 0 donné en paramètre, renvoie le coût du niveau actuel).
* @return cost Prix de l'achat/amélioration.
*/
int Slingshot::getCost(unsigned int level) const{

    // Si aucune valeur en argument, alors on affiche le coût du niveau courant
    if(level == 0) level = _level;

    switch(level) {
        case 1: return BASIC_COST;
        case 2: return INTERMEDIATE_COST;
        default: return HIGHEST_COST;
    }
}


/**
* @brief Renvoie un texte contenant des informations sur le défenseur (niveau, puissance, amélioration etc...).
* @return infos Texte contenant les informations.
*/
QString Slingshot::getInfos(void) const {

    // Infos générales
    QString infos = "Lance-pierres de niveau ";
    infos+= QString::number(_level);
    // Amélioration
    if(!this->isLevelMax()) {
        infos+= "\n- Amelioration : ";
        infos+= QString::number(this->getCost(_level+1));
    }
    // Revente
    infos+= "\n- Revente : ";
    infos+= QString::number(this->getCost(_level)/2);

    return infos;
}


/**
* @brief Met à jours les caractéristiques du défenseur, en se basant sur son niveau actuel.
*/
void Slingshot::updateStats(void) {

    _range = (3.0f + _level/2.0f)*32.0f;
    _rate = 1;
    _power = 10 * _level * sqrt(_level); // level ^ 1.5

}


/**
* @brief Méthode propre au QGraphicsItem appelée lorsqu'il est nécessaire de redessiner le défenseur
* @param painter Objet de dessin utilisé par la scène
*/
void Slingshot::paint(QPainter *painter, const QStyleOptionGraphicsItem *so, QWidget *w) {

    // Dessine la portée de la tourelle si celle ci est selectionnée
    Defenser::paint(painter,so,w);

    // Dessine la tourelle
    painter->setBrush(QBrush(Qt::gray));
    painter->drawRect(0,0,32,32);
}


/**
* @brief Tire un projectile sur la cible actuellement enregistrée.
* Le tir est effectué selon le compteur interne de fréquence de tir.
*/
void Slingshot::shootTarget(void) {
    if (!isShooting())
        return;

    _shootTimerStep+= 1;

    if (_shootTimerStep < (GAME::FPS / _rate))
        return;

    _shootTimerStep = 0;

    // Créé un nouveau projectile au niveau du centre de la tourelle
    Projectile* shot = new ProjectileStone(this->x()+14,this->y()+14,_targetX,_targetY,_power*_powerBonus, _map);
    _map->addItem(shot);

}



/************************************************************************************************************
* PAINT-BALL
************************************************************************************************************/

/**
* @brief Constructeur
* @param posx Position x du défenseur, doit être multiple de 32.
* @param posy Position y du défenseur, doit être multiple de 32.
* @param level Niveau de départ de la tourelle (entier positif)
* @param map Pointeur sur la map contenant le défenseur
*/
Paintball::Paintball(int posx,int posy,unsigned int level,Map* map)
    : Defenser(posx,posy,level,map)
{
    // Type de cible
    _targetType = DEFAULT_TARGET_TYPE;
    _shootTimerStep = 0;
    // Caractéristiques
    this->updateStats();
}


/**
* @brief Renvoie le coût nécessaire à l'achat/amélioration
* @param level Niveau dont on souhaite avoir le prix (si 0 donné en paramètre, renvoie le coût du niveau actuel).
* @return cost Prix de l'achat/amélioration.
*/
int Paintball::getCost(unsigned int level) const{

    // Si aucune valeur en argument, alors on affiche le coût du niveau courant
    if(level == 0) level = _level;

    switch(level) {
        case 1: return BASIC_COST;
        case 2: return INTERMEDIATE_COST;
        default: return HIGHEST_COST;
    }
}


/**
* @brief Renvoie un texte contenant des informations sur le défenseur (niveau, puissance, amélioration etc...).
* @return infos Texte contenant les informations.
*/
QString Paintball::getInfos(void) const {

    // Infos générales
    QString infos = "Paint-ball de niveau ";
    infos+= QString::number(_level);
    // Amélioration
    if(!this->isLevelMax()) {
        infos+= "\n- Amelioration : ";
        infos+= QString::number(this->getCost(_level+1));
    }
    // Revente
    infos+= "\n- Revente : ";
    infos+= QString::number(this->getCost(_level)/2);

    return infos;
}


/**
* @brief Met à jours les caractéristiques du défenseur, en se basant sur son niveau actuel.
*/
void Paintball::updateStats(void) {

    _range = (4.0f + _level/2.0f)*32.0f;
    _rate = 2;
    _power = 4 * _level * sqrt(_level); // level ^ 1.5

}


/**
* @brief Méthode propre au QGraphicsItem appelée lorsqu'il est nécessaire de redessiner le défenseur
* @param painter Objet de dessin utilisé par la scène
*/
void Paintball::paint(QPainter *painter, const QStyleOptionGraphicsItem *so, QWidget *w) {

    // Dessine la portée de la tourelle si celle ci est selectionnée
    Defenser::paint(painter,so,w);

    // Dessine la tourelle
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(0,0,32,32);


}


/**
* @brief Tire un projectile sur la cible actuellement enregistrée.
* Le tir est effectué selon le compteur interne de fréquence de tir.
*/
void Paintball::shootTarget(void) {
    if (!isShooting())
        return;

    _shootTimerStep+= 1;

    if (_shootTimerStep < (GAME::FPS / _rate))
        return;

    _shootTimerStep = 0;

    // Créé un nouveau projectile au niveau du centre de la tourelle
    Projectile* shot = new ProjectilePaintball(this->x()+14,this->y()+14,_targetX,_targetY,_power*_powerBonus, _map);
    _map->addItem(shot);

}




/************************************************************************************************************
* PETANQUE
************************************************************************************************************/

/**
* @brief Constructeur
* @param posx Position x du défenseur, doit être multiple de 32.
* @param posy Position y du défenseur, doit être multiple de 32.
* @param level Niveau de départ de la tourelle (entier positif)
* @param map Pointeur sur la map contenant le défenseur
*/
Bowling::Bowling(int posx,int posy,unsigned int level,Map* map)
    : Defenser(posx,posy,level,map)
{
    // Type de cible
    _targetType = DEFAULT_TARGET_TYPE;
    _shootTimerStep = 0;
    // Caractéristiques
    this->updateStats();
}


/**
* @brief Renvoie le coût nécessaire à l'achat/amélioration
* @param level Niveau dont on souhaite avoir le prix (si 0 donné en paramètre, renvoie le coût du niveau actuel).
* @return cost Prix de l'achat/amélioration.
*/
int Bowling::getCost(unsigned int level) const{

    // Si aucune valeur en argument, alors on affiche le coût du niveau courant
    if(level == 0) level = _level;

    switch(level) {
        case 1: return BASIC_COST;
        case 2: return INTERMEDIATE_COST;
        default: return HIGHEST_COST;
    }
}


/**
* @brief Renvoie un texte contenant des informations sur le défenseur (niveau, puissance, amélioration etc...).
* @return infos Texte contenant les informations.
*/
QString Bowling::getInfos(void) const {

    // Infos générales
    QString infos = "Joueur de petanque de niveau ";
    infos+= QString::number(_level);
    // Amélioration
    if(!this->isLevelMax()) {
        infos+= "\n- Amelioration : ";
        infos+= QString::number(this->getCost(_level+1));
    }
    // Revente
    infos+= "\n- Revente : ";
    infos+= QString::number(this->getCost(_level)/2);

    return infos;
}


/**
* @brief Met à jours les caractéristiques du défenseur, en se basant sur son niveau actuel.
*/
void Bowling::updateStats(void) {

    _range = (3.0f + _level/2.0f)*32.0f;
    _rate = 0.5;
    _power = 15 * _level * sqrt(_level); // level ^ 1.5

}


/**
* @brief Méthode propre au QGraphicsItem appelée lorsqu'il est nécessaire de redessiner le défenseur
* @param painter Objet de dessin utilisé par la scène
*/
void Bowling::paint(QPainter *painter, const QStyleOptionGraphicsItem *so, QWidget *w) {

    // Dessine la portée de la tourelle si celle ci est selectionnée
    Defenser::paint(painter,so,w);

    // Dessine la tourelle
    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(0,0,32,32);

}


/**
* @brief Tire un projectile sur la cible actuellement enregistrée.
* Le tir est effectué selon le compteur interne de fréquence de tir.
*/
void Bowling::shootTarget(void) {
    if (!isShooting())
        return;

    _shootTimerStep+= 1;

    if (_shootTimerStep < (GAME::FPS / _rate))
        return;

    _shootTimerStep = 0;

    // Créé un nouveau projectile au niveau du centre de la tourelle
    Projectile* shot = new ProjectileBowling(this->x()+14,this->y()+14,_targetX,_targetY,_power*_powerBonus, _map);
    _map->addItem(shot);

}



/************************************************************************************************************
* MUSICIEN
************************************************************************************************************/

/**
* @brief Constructeur.
* @param posx Position x du défenseur, doit être multiple de 32.
* @param posy Position y du défenseur, doit être multiple de 32.
* @param level Niveau de départ de la tourelle (entier positif).
* @param map Pointeur sur la map contenant le défenseur.
*/
Musician::Musician(int posx,int posy,unsigned int level,Map* map)
    : Defenser(posx,posy,level,map)
{
    // Caractéristiques
    _range = 1; // portée de 1 case
    this->updateStats();
    // Mise à jour auto des bonus
    QObject::connect(_map,SIGNAL(updateBonuses()),this,SLOT(applyBonuses()));
}


/**
* @brief Renvoie le coût nécessaire à l'achat/amélioration.
* @param level Niveau dont on souhaite avoir le prix (si 0 donné en paramètre, renvoie le coût du niveau actuel).
* @return cost Prix de l'achat/amélioration.
*/
int Musician::getCost(unsigned int level) const{

    // Si aucune valeur en argument, alors on affiche le coût du niveau courant
    if(level == 0) level = _level;

    switch(level) {
        case 1: return BASIC_COST;
        case 2: return INTERMEDIATE_COST;
        default: return HIGHEST_COST;
    }
}


/**
* @brief Renvoie un texte contenant des informations sur le défenseur (niveau, puissance, amélioration etc...).
* @return infos Texte contenant les informations.
*/
QString Musician::getInfos(void) const {

    // Infos générales
    QString infos = "Musicien de niveau ";
    infos+= QString::number(_level);
    // Amélioration
    if(!this->isLevelMax()) {
        infos+= "\n- Amelioration : ";
        infos+= QString::number(this->getCost(_level+1));
    }
    // Revente
    infos+= "\n- Revente : ";
    infos+= QString::number(this->getCost(_level)/2);

    return infos;
}


/**
* @brief Met à jours les caractéristiques du défenseur, en se basant sur son niveau actuel.
*/
void Musician::updateStats(void) {
    _powerBonus = 1+_level*0.2f; // Pourcentage de bonus (>1)
}


/**
* @brief Méthode propre au QGraphicsItem appelée lorsqu'il est nécessaire de redessiner le défenseur.
* @param painter Objet de dessin utilisé par la scène.
*/
void Musician::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) {

    // Si la tourelle est selectionnée...
    if(_isSelected) {

        painter->save();

        // Dessine la portée de la tourelle
        painter->setOpacity(0.2f);
        painter->setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
        painter->drawRect(-16*2,-16*2,32*_range*3,32*_range*3);

        painter->restore();
    }

    // Dessine la tourelle
    painter->setBrush(QBrush(Qt::yellow));
    painter->drawRect(0,0,32,32);
}


// Masquage de méthode
void Musician::advance(int){}


/**
* @brief Renvoie le pourcentage du bonus apporté par le musicien.
* @return _powerBonus Bonus d'attaque (>=1)
*/
float Musician::getPowerBonus(void) const { return _powerBonus; }


/**
* @brief Recherche les tourelles voisines pour appliquer le bonus du musicien.
* Cette méthode peut être appellée de manière générale via un signal
* ou de manière indépendante lors de l'ajout/suppression d'une défenseur.
*/
void Musician::applyBonuses(void) {

    Defenser* bonusTarget;

    for(int i = -_range ; i <= _range ; ++i) {

        for(int j = -_range ; j <= _range ; ++j ) {

            // Cas particulier où l'on est au centre (== Musicien lui même)
            if(i==0 && j==0)
                continue;

            // On applique un bonus à l'éventuelle tourelle voisine
            // NB: si le bonus appliqué est inférieur à un autre bonus, seul le bonus le plus fort sera conservé
            if((bonusTarget = _map->getTurretAt(this->x()+32*i,this->y()+32*j)) != NULL) {
                bonusTarget->setBonus(_powerBonus);
            }

        } //eof
    }// eof

}


/**
* @brief Recherche les tourelles voisines pour appliquer leur retirer le bonus du musicien.
* Cette méthode peut être appellée de manière générale via un signal
* ou de manière indépendante lors de l'ajout/suppression d'une défenseur.
* Attention, l'appel à cette méthode doit être suivi d'un appel via signal global
* à la méthode applyBonus des éventuels autres Musiciens (cas de voisins Musiciens multiples)
*/
void Musician::removeBonuses(void) {

    Defenser* bonusTarget;

    for(int i = -_range ; i <= _range ; ++i) {

        for(int j = -_range ; j <= _range ; ++j ) {

            // Cas particulier où l'on est au centre (== Musicien lui même)
            if(i==0 && j==0)
                continue;

            // Bonus remis à la valeur initiale
            if((bonusTarget = _map->getTurretAt(this->x()+32*i,this->y()+32*j)) != NULL && dynamic_cast<Musician*>(bonusTarget) == 0)
                bonusTarget->removeBonus();
        } //eof
    }// eof

}

// Masquage de méthode
void Musician::shootTarget(void) {}

