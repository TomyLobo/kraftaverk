#include "mainwindow.h"
#include "glwidget.h"
#include "dockblockicon.h"
#include "world.h"

#include <QGridLayout>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    mWorld(0)
{
    setWindowTitle("kraftaverk");

    glWidget = new GLWidget;
    setCentralWidget(glWidget);

    QDockWidget * dock = new QDockWidget("Blocks", this);
    dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    DockBlockIcon * dbi = new DockBlockIcon(dock);
    dock->setWidget(dbi);
    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetVerticalTitleBar);
    addDockWidget(Qt::BottomDockWidgetArea, dock);

    connect(glWidget, SIGNAL(wheel(int)), dbi, SLOT(shiftIcon(int)));
    connect(dbi, SIGNAL(activeBlockTypeChanged(Block::BlockType)), glWidget, SLOT(setCurrentBlockType(Block::BlockType)));

    initWorld("rs_latch.red");
    initMenus();

    resize(640, 480);
}

MainWindow::~MainWindow()
{}

void MainWindow::initWorld(const QString &file)
{
    //glWidget->setWorld(0);
    glWidget->makeCurrent();
    delete mWorld;
    mWorld = new World(file);
    glWidget->setWorld(mWorld);
}

void MainWindow::initMenus()
{
    QAction * item;
    QMenu * fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    {
        item = new QAction("&Load", this);
        fileMenu->addAction(item);

        QFileDialog * dialog = new QFileDialog(this, "Load circuit", QDir::currentPath());
        dialog->setFileMode(QFileDialog::ExistingFile);

        connect(item, SIGNAL(triggered()), dialog, SLOT(open()));
        connect(dialog, SIGNAL(fileSelected(QString)), SLOT(initWorld(QString)));
    }

    {
        item = new QAction("E&xit", this);
        fileMenu->addAction(item);

        connect(item, SIGNAL(triggered()), SLOT(close()));
    }
}
