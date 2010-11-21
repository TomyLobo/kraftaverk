#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "world.h"
#include "mathlib/matrix4.h"

#include <QGLWidget>

class GLWidget : public QGLWidget
{
    Q_OBJECT
private:
    World * world;
    QPointF lastPos;

public:
    explicit GLWidget(QWidget *parent = 0);
    void setWorld(World * world);
    void setYaw(qreal yaw);

protected:
    virtual void mousePressEvent(QMouseEvent * mouseEvent);
    virtual void mouseMoveEvent(QMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QMouseEvent * mouseEvent);

    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

signals:
    void mouseDragged(QPointF const & delta);

private slots:
    void resetWorld() { this->world = 0; }
};

#endif // GLWIDGET_H
