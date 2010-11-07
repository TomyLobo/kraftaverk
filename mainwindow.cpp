#include "mainwindow.h"

#include "blockstone.h"

#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Kraftaverk");

    QGridLayout * layout = new QGridLayout;

    mScene = new QGraphicsScene;
    mView = new QGraphicsView(mScene);

    layout->addWidget(mView, 0, 0);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);
    resize(640, 480);

    initWorld();
}

void MainWindow::initWorld() {
    mWorld = new World(mScene);

    new BlockStone(Position(0, 0, 0), mWorld);

    mWorld->updateGeometry();
}

MainWindow::~MainWindow()
{

}
