#include "mainwindow.h"

#include "graphicsscene.h"

#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    mWorld(0)
{
    setWindowTitle("kraftaverk");

    QGridLayout * layout = new QGridLayout;

    mScene = new GraphicsScene;
    mView = new QGraphicsView(mScene);
    //mView->setRenderHint(QPainter::Antialiasing, true);

    layout->addWidget(mView, 0, 0);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);
    resize(640, 480);

    initWorld();
}

void MainWindow::initWorld() {
    mWorld = new World("D:/Data/Dokumente/minecraft/rs_latch.red", mScene);

    mWorld->updateGeometry();
}

MainWindow::~MainWindow()
{

}
