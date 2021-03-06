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
    Map* _sceneMap;
    Player* _player;
    GAME::DIFFICULTY _gameDifficulty;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void launchGame(QWidget *parent);

public slots:
    void showMapContextMenu(const QPoint& pos);
    void endGame(QString msg);
    void reloadGame(void);
    void setGameModeEasy(void);
    void setGameModeHard(void);

};

#endif // MAINWINDOW_H
