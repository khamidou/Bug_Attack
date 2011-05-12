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

    _currentMap = new Map(_scene);
    _currentMap->LoadBackground();

    _view->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
