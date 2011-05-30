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
    _player = new Player(999,10); // 22 - 10


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
    _view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);


    _view->show();


    /**
    * Interface graphique
    **/
    /// Lancement de la prochaine vague
    // . Lance la production d'ennemi
    QObject::connect(ui->launchWaveButton, SIGNAL(pressed()),_sceneMap->getWaveGenerator(),SLOT(launchWaves()));
    // . Change le texte d'ambiance associé à la vague courante
    QObject::connect(_sceneMap->getWaveGenerator(),SIGNAL(changeWaveDescLabel(QString)),ui->waveDescLabel,SLOT(setText(QString)));
    _sceneMap->getWaveGenerator()->getNextWaveDesc();
    // . Rend le bouton inactif le temps que la vague se produise
    QObject::connect(_sceneMap->getWaveGenerator(),SIGNAL(setLaunchWaveButtonEnabled(bool)),ui->launchWaveButton,SLOT(setEnabled(bool)));


    /// Boutons de choix de tourelle
    // . Pistolet à eau
    QObject::connect(ui->turretButton_PistoletAEau,SIGNAL(pressed()),_player,SLOT(setTurretChoice1()));
    QObject::connect(_player,SIGNAL(setTurret1ButtonDisabled(bool)),ui->turretButton_PistoletAEau,SLOT(setDisabled(bool)));
    // . Lance-pierres
    QObject::connect(ui->turretButton_LancePierres,SIGNAL(pressed()),_player,SLOT(setTurretChoice2()));
    QObject::connect(_player,SIGNAL(setTurret2ButtonDisabled(bool)),ui->turretButton_LancePierres,SLOT(setDisabled(bool)));
    // . Paintball
    QObject::connect(ui->turretButton_Paintball,SIGNAL(pressed()),_player,SLOT(setTurretChoice3()));
    QObject::connect(_player,SIGNAL(setTurret3ButtonDisabled(bool)),ui->turretButton_Paintball,SLOT(setDisabled(bool)));
    // . Pétanque
    QObject::connect(ui->turretButton_Petanque,SIGNAL(pressed()),_player,SLOT(setTurretChoice4()));
    QObject::connect(_player,SIGNAL(setTurret4ButtonDisabled(bool)),ui->turretButton_Petanque,SLOT(setDisabled(bool)));
    // . Musicien
    QObject::connect(ui->turretButton_Musicien,SIGNAL(pressed()),_player,SLOT(setTurretChoice5()));
    QObject::connect(_player,SIGNAL(setTurret5ButtonDisabled(bool)),ui->turretButton_Musicien,SLOT(setDisabled(bool)));

    // Demande d'informations sur une tourelle
    QObject::connect(_sceneMap,SIGNAL(turretInfosRequest(QString)),ui->turretInfoLabel,SLOT(setText(QString)));
    // Amélioration d'une tourelle
    QObject::connect(ui->upgradeTurretButton,SIGNAL(clicked()),_sceneMap,SLOT(upgradeTurret()));
    // Revente d'une tourelle
    QObject::connect(ui->sellTurretButton,SIGNAL(clicked()),_sceneMap,SLOT(removeTurret()));
    QObject::connect(_sceneMap,SIGNAL(turretSold(int)),_player,SLOT(turretSold(int)));
    // Activation ~ Désactivation des boutons
    ui->upgradeTurretButton->setDisabled(true);
    ui->sellTurretButton->setDisabled(true);
    QObject::connect(_sceneMap,SIGNAL(disableTurretUpgradeButton(bool)),ui->upgradeTurretButton,SLOT(setDisabled(bool)));
    QObject::connect(_sceneMap,SIGNAL(disableTurretSellButton(bool)),ui->sellTurretButton,SLOT(setDisabled(bool)));


    /// Gestions des compteurs
    // Argent
    ui->creditCounter->display(_player->getMoney()); // C'est du vol
    QObject::connect(_player,SIGNAL(moneyValueChanged(int)),ui->creditCounter,SLOT(display(int)));
    // Vies
    ui->livesCounter->display(_player->getLives());
    QObject::connect(_player,SIGNAL(livesValueChanged(int)),ui->livesCounter,SLOT(display(int)));

    // Gameover
    QObject::connect(_player,SIGNAL(gameLost(QString)),this,SLOT(endGame(QString)));
    QObject::connect(_sceneMap->getWaveGenerator(),SIGNAL(gameWon(QString)),this,SLOT(endGame(QString)));

    /**
    * Divers
    **/
    // Affichage d'un menu de choix de tourelle au clic droit sur la map
    _view->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(_view,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showMapContextMenu(const QPoint&)));
    QObject::connect(ui->pauseButton,SIGNAL(clicked()), _sceneMap,SLOT(setPause(void)));
}

void MainWindow::showMapContextMenu(const QPoint& pos) {

   // On récupère la position du clic (par rapport à la mainWindow puis par rapport à la QGraphicsView
   QPoint globalPos = _view->viewport()->mapToGlobal(pos);
   QPoint mousePos = _view->viewport()->mapToParent(pos);


   // Création du menu de choix de la tourelle
   QMenu myMenu(this);
   QAction* createTurret1 = myMenu.addAction("Pistolet a eau");
   QAction* createTurret2 = myMenu.addAction("Lance-pierres");
   QAction* createTurret3 = myMenu.addAction("Paintball");
   QAction* createTurret4 = myMenu.addAction("Petanque");
   QAction* createTurret5 = myMenu.addAction("Musicien");

   // On regarde si un choix a été effectué et si la case est libre pour la pose d'une tourelle
   QAction* selectedItem = myMenu.exec(globalPos);
   if (selectedItem && _sceneMap->getTurretAt((int)(mousePos.x()/32)*32,(int)(mousePos.y()/32)*32)==NULL)
   {
       // TODO CHANGER ADD TURRET
       if(selectedItem == createTurret1)
           _sceneMap->addTurretAt(TYPE::PISTOLET_A_EAU,mousePos.x(),mousePos.y());
       else if(selectedItem == createTurret2)
           _sceneMap->addTurretAt(TYPE::LANCE_PIERRES,mousePos.x(),mousePos.y());
       else if(selectedItem == createTurret3)
           _sceneMap->addTurretAt(TYPE::PAINTBALL,mousePos.x(),mousePos.y());
       else if(selectedItem == createTurret4)
           _sceneMap->addTurretAt(TYPE::PETANQUE,mousePos.x(),mousePos.y());
       else if(selectedItem == createTurret5)
           _sceneMap->addTurretAt(TYPE::MUSICIEN,mousePos.x(),mousePos.y());
   }

}

void MainWindow::endGame(QString msg) {

    QMessageBox msgBox;
    msgBox.setText("Fin de partie");
    msgBox.setInformativeText(msg);

    QPushButton *restartButton = msgBox.addButton(tr("Rejouer"), QMessageBox::ActionRole);
    QPushButton *quitButton = msgBox.addButton(tr("Quitter"), QMessageBox::ActionRole);
    msgBox.exec();

    // Relance une nouvelle partie
    if (msgBox.clickedButton() == (QAbstractButton *) restartButton) {

    }
    // Quitte l'application
    else if (msgBox.clickedButton() == (QAbstractButton *) quitButton) {
         this->close();
    }


}

MainWindow::~MainWindow(void)
{
    delete ui;
}
