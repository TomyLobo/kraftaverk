#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "world.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QGraphicsView * mView;
    QGraphicsScene * mScene;
    World * mWorld;

    void initWorld();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
