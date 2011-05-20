#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    // Création de la scène (map)
    _sceneMap = new Map(parent);
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
    */
    // Lancement de la prochaine vague
    // . Lance la production d'ennemi
    QObject::connect(ui->launchWaveButton, SIGNAL(pressed()),_sceneMap->getWaveGenerator(),SLOT(launchWaves()));
    // . Change le texte d'ambiance associé à la vague courante
    QObject::connect(_sceneMap->getWaveGenerator(),SIGNAL(changeWaveDescLabel(QString)),ui->waveDescLabel,SLOT(setText(QString)));
    _sceneMap->getWaveGenerator()->getNextWaveDesc();
    // . Rend le bouton inactif le temps que la vague se produise
    QObject::connect(_sceneMap->getWaveGenerator(),SIGNAL(setLaunchWaveButtonEnabled(bool)),ui->launchWaveButton,SLOT(setEnabled(bool)));

}


MainWindow::~MainWindow()
{
    delete ui;
}
