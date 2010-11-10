#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "world.h"
#include "graphicsscene.h"

#include <QMainWindow>
#include <QGraphicsView>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QGraphicsView * mView;
    GraphicsScene * mScene;
    World * mWorld;
    qreal mYaw;

    void initWorld();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void sceneMouseDragged(QPointF const & delta);
};

#endif // MAINWINDOW_H
