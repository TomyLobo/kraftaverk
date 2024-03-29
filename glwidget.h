#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "position.h"
#include "mathlib/angle.h"
#include "block.h"

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
    bool mDirty;
    Block::BlockType currentBlockType;

    void mouseDragged(QPointF const & delta);

public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
    void setWorld(World * world);
    void setAngle(angle<qreal> const & angle);
    void setDirty();

protected:
    virtual void mousePressEvent(QMouseEvent * mouseEvent);
    virtual void mouseMoveEvent(QMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QMouseEvent * mouseEvent);
    virtual void wheelEvent(QWheelEvent * event);
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void focusOutEvent(QFocusEvent * event);

    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private slots:
    void resetWorld() { setWorld(0); }

signals:
    void wheel(int direction);

public slots:
    void setCurrentBlockType(Block::BlockType newBlockType);
};

#endif // GLWIDGET_H
