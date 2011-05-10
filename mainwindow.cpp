#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    /// Test : try to draw some tiles into the QGraphicView

    _scene = new QGraphicsScene(parent);
    _view = ui->graphicsView;
    _view->setScene(_scene);

    QPixmap grass("media/textures/grass.jpg");
    grass = grass.scaled(32,32,Qt::IgnoreAspectRatio);
    for(int i = 0; i < 16; ++i){
        for(int j = 0; j < 16 ; ++j){

            _grassTile = _scene->addPixmap(grass);
            _grassTile->setPos(32*i,32*j);

        }
     }

    _view->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}
