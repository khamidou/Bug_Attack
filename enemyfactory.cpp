#include "enemyfactory.h"
#include <iostream>

WaveElement::WaveElement(QString data) {

    // Extraction des données
    QList<QString> subElementDesc = data.split(':');

    // Création des wave elements
    // . TYPE
    if(subElementDesc.first() == "cafard")
        _type = TYPE::CAFARD;
    else if(subElementDesc.first() == "fourmi")
        _type = TYPE::FOURMI;
    else if(subElementDesc.first() == "guepe")
        _type = TYPE::GUEPE;
    else if(subElementDesc.first() == "moustique")
        _type = TYPE::MOUSTIQUE;
    else
        _type = TYPE::CAFARD;
    subElementDesc.pop_front();
    // . TAILLE
    _size = subElementDesc.first().toFloat();
    subElementDesc.pop_front();
    // . NOMBRE
    _number = subElementDesc.first().toInt();
    subElementDesc.pop_front();
    // . FREQUENCE
    _frequence = subElementDesc.first().toInt();
}

TYPE::ENEMY WaveElement::getType(void) const{return _type;}
float WaveElement::getSize(void) const{return _size;}
int WaveElement::getNumber(void) const{return _number;}
int WaveElement::getFrequence(void) const{return _frequence;}
void WaveElement::decreaseNumber(void) { if(_number>0) --_number; }

Wave::Wave(QString data) {

    // Extraction des données
    QList<QString> waveDesc = data.split(";");

    // Titre de la vague courante
    _desc = waveDesc.first();
    waveDesc.pop_front();

    // Création des wave elements
    QList<QString>::iterator i;
    for(i = waveDesc.begin() ; i != waveDesc.end() ; ++i)
        _waveElements.push_back(new WaveElement(*i));

}

WaveElement* Wave::popNextWaveElement(void){

    // Les éléments de la vague courantes ont été crées (erreur)
    if(_waveElements.empty())
        return NULL;

    // On récupére l'élement courant
    WaveElement* result = _waveElements.first();

    // Si le nombre d'élément restant est nul, on passe au suivant
    if(result->getNumber() == 1){
        _waveElements.pop_front();
    }

    // On retire l'élément courant qui va être créé
    result->decreaseNumber();

    return result;
}

bool Wave::isFinished(void) const { return(_waveElements.empty()); }
QString Wave::getDesc(void) const { return _desc; }

EnemyFactory::EnemyFactory(Map* map):_currentMap(map)
{

    // Lit le fichier wave correspondant à la map courante
    QFile file("./media/maps/waves.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    // Extrait chaque vague d'ennemis
    while(!file.atEnd()){
        QString line = file.readLine();
        _mapWaves.push_back(new Wave(line));

    }
    // Ferme le fichier
    file.close();

    _waveTimerStep = 0;    
}

void EnemyFactory::productEnemy(WaveElement* enemy){

    Enemy* newEnemy;
    QPoint start = _currentMap->getStart();

    // Crée l'ennemi correspondant à la description du WaveElement
    switch(enemy->getType()) {

    case TYPE::CAFARD:
        newEnemy = new Cafard(_currentMap,start.x(),start.y(),enemy->getSize());
        break;
    case TYPE::FOURMI:
        newEnemy = new Fourmi(_currentMap,start.x(),start.y(),enemy->getSize());
        break;
    case TYPE::GUEPE:
        newEnemy = new Guepe(_currentMap,start.x(),start.y(),enemy->getSize());
        break;
    case TYPE::MOUSTIQUE:
        newEnemy = new Moustique(_currentMap,start.x(),start.y(),enemy->getSize());
        break;
    default:
        newEnemy = new Cafard(_currentMap,start.x(),start.y(),enemy->getSize());
    }

}

void EnemyFactory::getNextWaveDesc(void){
    if(!_mapWaves.empty())
        emit changeWaveDescLabel(QString(_mapWaves.first()->getDesc()));
}

void EnemyFactory::launchWaves(void){
    // Désactive le bouton de lancement de vague ennemie
    emit setLaunchWaveButtonEnabled(false);
    // Lance la production
    connect(&_currentMap->gameTimer,SIGNAL(timeout()),this,SLOT(produceWave(void)));
}

void EnemyFactory::produceWave(void){
    _waveTimerStep+= 1; /* pas très sur de ça */

    if (_waveTimerStep < (GAME::FPS / 2)) // FIXME : ajuster la vitesse.
        return;

    _waveTimerStep = 0;
    // Vérifie s'il reste des vagues à produire
    if(_mapWaves.empty()){
        if(_currentMap->getEnemyList().empty()) {

            // Fin de partie : Gagné !
            emit gameWon(QString("Vous avez gagné"));

        }
        return;
    }

    // Récupère la vague courante
    if(_mapWaves.first()->isFinished()){

        // Charge la vague suivante
        _mapWaves.pop_front();        

        // Ré-active le bouton de lancement de vague ennemie
        // ... s'il reste encore des vagues à lancer
        if(!_mapWaves.empty()){
            emit setLaunchWaveButtonEnabled(true);
            //.. et affiche le contenu de la prochaine vague
            this->getNextWaveDesc();
            disconnect(&_currentMap->gameTimer,SIGNAL(timeout()),this,SLOT(produceWave(void)));
        }

        return;
    }

    // Produit le contenu de cette vague
    WaveElement* nextEnemy = _mapWaves.first()->popNextWaveElement();
    if(nextEnemy != NULL) { // sécurité (TODO facultatif ?)        
        this->productEnemy(nextEnemy);
    }
}




