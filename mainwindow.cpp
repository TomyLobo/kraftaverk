#include "mainwindow.h"

#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    mWorld(0),
    mYaw(0)
{
    setWindowTitle("kraftaverk");

    glWidget = new GLWidget;

    connect(glWidget, SIGNAL(mouseDragged(QPointF)), SLOT(sceneMouseDragged(QPointF)));

    setCentralWidget(glWidget);
    resize(640, 480);

    initWorld();

}

MainWindow::~MainWindow()
{}

void MainWindow::initWorld() {
    mWorld = new World("rs_latch.red");
    glWidget->setWorld(mWorld);
}

void MainWindow::sceneMouseDragged(QPointF const & delta)
{
    if (!mWorld) return;

    mYaw += delta.x() * .01;
    glWidget->setYaw(mYaw);
}
