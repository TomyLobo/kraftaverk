#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GLWidget;
class World;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    GLWidget * glWidget;
    World * mWorld;

    void initMenus();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void initWorld(const QString &file);
};

#endif // MAINWINDOW_H
