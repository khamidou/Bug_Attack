#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "map.h"
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include <QTimer>
#include <QTime>

#include "map.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    static const int FPS = 30;

    QGraphicsView *_view;
    QTimer timer;
    Map* _sceneMap;

};

#endif // MAINWINDOW_H
