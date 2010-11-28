#ifndef DOCKBLOCKICON_H
#define DOCKBLOCKICON_H

#include <QGLWidget>
#include <QTime>

class DockBlockIcon : public QGLWidget
{
    Q_OBJECT
private:
    QList<class Block *> icons;
    QTime time;
    int mActiveIcon;

public:
    explicit DockBlockIcon(QWidget *parent = 0);
    ~DockBlockIcon();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    virtual void mousePressEvent(QMouseEvent * mouseEvent);
    virtual void timerEvent(QTimerEvent * event);

private:
    void drawIcon(int index, qreal yaw, bool highlighted, bool selected);
};

#endif // DOCKBLOCKICON_H
