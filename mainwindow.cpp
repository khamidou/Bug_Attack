#include <QMenu>
#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /**
    * Initialisation des données du joueur
    **/
    _player = new Player(20,10);

    /**
    * Création et chargement de la map
    **/
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    // Création de la scène (map)
    _sceneMap = new Map(parent,_player);
    _view = ui->graphicsView;
    _view->setScene(_sceneMap);
    _view->setRenderHint(QPainter::Antialiasing);
    _view->setCacheMode(QGraphicsView::CacheBackground);
    _view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    _view->show();

    // Lancement du timer (boucle principale du jeu)
    QObject::connect(&timer, SIGNAL(timeout()), _sceneMap, SLOT(advance()));
    timer.start(1000 / FPS);

    /**
    * Interface graphique
    **/
    // Lancement de la prochaine vague
    // . Lance la production d'ennemi
    QObject::connect(ui->launchWaveButton, SIGNAL(pressed()),_sceneMap->getWaveGenerator(),SLOT(launchWaves()));
    // . Change le texte d'ambiance associé à la vague courante
    QObject::connect(_sceneMap->getWaveGenerator(),SIGNAL(changeWaveDescLabel(QString)),ui->waveDescLabel,SLOT(setText(QString)));
    _sceneMap->getWaveGenerator()->getNextWaveDesc();
    // . Rend le bouton inactif le temps que la vague se produise
    QObject::connect(_sceneMap->getWaveGenerator(),SIGNAL(setLaunchWaveButtonEnabled(bool)),ui->launchWaveButton,SLOT(setEnabled(bool)));

    // Gestions des compteurs
    ui->creditCounter->display(_player->getMoney()); // C'est du vol
    QObject::connect(_player,SIGNAL(moneyValueChanged(int)),ui->creditCounter,SLOT(display(int)));
   // QObject::connect(_sceneMap,SIGNAL(enemyDestroyed(int)),_player,SLOT(earnMoney(int)));

    /**
    * Divers
    **/
    // Affichage d'un menu de choix de tourelle au clic droit sur la map
    _view->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(_view,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showMapContextMenu(const QPoint&)));

}

void MainWindow::showMapContextMenu(const QPoint& pos) {

   // On récupère la position du clic (par rapport à la mainWindow puis par rapport à la QGraphicsView
   QPoint globalPos = _view->viewport()->mapToGlobal(pos);
   QPoint mousePos = _view->viewport()->mapToParent(pos);


   // Création du menu de choix de la tourelle
   QMenu myMenu(this);
   QAction* createTurret1 = myMenu.addAction("Pistolet a eau");
   myMenu.addAction("-- turret 2 --");
   myMenu.addAction("-- turret 3 --");
   myMenu.addAction("-- turret 4 --");
   myMenu.addAction("-- turret 5 --");

   // Récupération d'un éventuel choix
   QAction* selectedItem = myMenu.exec(globalPos);
   if (selectedItem)
   {
       // TODO CHANGER ADD TURRET
       if(selectedItem == createTurret1)
            _sceneMap->addTurretAt(mousePos.x(),mousePos.y());
   }

}

MainWindow::~MainWindow()
{
    delete ui;
}
