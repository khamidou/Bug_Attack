#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    /// Test : try to draw some tiles into the QGraphicView

    _sceneMap = new Map(parent);
    _view = ui->graphicsView;
    _view->setScene(_sceneMap);
    _view->setRenderHint(QPainter::Antialiasing);
    _view->setCacheMode(QGraphicsView::CacheBackground);
    _view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    _view->show();

    // Lancement du timer (boucle principale du jeu)
    QObject::connect(&timer, SIGNAL(timeout()), _sceneMap, SLOT(advance()));
    timer.start(1000 / 20); // 10 FPS


}


MainWindow::~MainWindow()
{
    delete ui;
}
