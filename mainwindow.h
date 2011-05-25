#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include <QTimer>
#include <QTime>

#include "types.h"
#include "map.h"
#include "player.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;

    QGraphicsView *_view;
    QTimer timer;
    Map* _sceneMap;
    Player* _player;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showMapContextMenu(const QPoint& pos);

};

#endif // MAINWINDOW_H
