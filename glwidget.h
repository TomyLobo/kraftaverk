#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "mathlib/matrix4.h"
#include "position.h"
#include "mathlib/angle.h"

#include <QGLWidget>

class World;

class GLWidget : public QGLWidget
{
    Q_OBJECT
private:
    World * world;
    QPointF lastPos;
    angle<qreal> mAngle;
    vec3 normalstart, normalvec;
    vec3 boxSize;

    void mouseDragged(QPointF const & delta);

public:
    explicit GLWidget(QWidget *parent = 0);
    void setWorld(World * world);
    void setAngle(angle<qreal> angle);

protected:
    virtual void mousePressEvent(QMouseEvent * mouseEvent);
    virtual void mouseMoveEvent(QMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QMouseEvent * mouseEvent);

    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private slots:
    void resetWorld() { setWorld(0); }
};

#endif // GLWIDGET_H
