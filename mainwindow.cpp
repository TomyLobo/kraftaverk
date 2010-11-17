#include "mainwindow.h"

#include "graphicsscene.h"

#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    mWorld(0),
    mYaw(0)
{
    setWindowTitle("kraftaverk");

    QGridLayout * layout = new QGridLayout;

    mScene = new GraphicsScene;

    connect(mScene, SIGNAL(mouseDragged(QPointF)), SLOT(sceneMouseDragged(QPointF)));

    mView = new QGraphicsView(mScene);
    mView->setRenderHint(QPainter::Antialiasing, true);

    layout->addWidget(mView, 0, 0);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);
    resize(640, 480);

    initWorld();

}

MainWindow::~MainWindow()
{}

void MainWindow::initWorld() {
    mWorld = new World("rs_latch.red", mScene);
}

void MainWindow::sceneMouseDragged(QPointF const & delta)
{
    if (!mWorld) return;

    mYaw += delta.x() * .01;
    mWorld->projector.setYaw(mYaw);
}
