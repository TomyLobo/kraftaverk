#include "glwidget.h"
#include "block.h"
#include "world.h"

#include "mathlib/matrix4.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::DirectRendering), parent),
    world(0),
    mDirty(true),
    currentBlockType(Block::btStone)
{
    setFocus(Qt::OtherFocusReason);
    setFocusPolicy(Qt::WheelFocus);
    setMouseTracking(true);
}

GLWidget::~GLWidget()
{
    makeCurrent();
}

#define DRAW_NORMAL

vec3 unProject(QPoint const & pos)
{
    GLdouble projection[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    GLdouble modelView[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelView);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    double winX = pos.x();
    double winY = viewport[3] - pos.y();

    GLfloat winZ;
    glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    vec3 obj;
    gluUnProject(winX, winY, winZ, modelView, projection, viewport, &obj.x, &obj.y, &obj.z);
    return obj;
}

void GLWidget::mousePressEvent(QMouseEvent * mouseEvent)
{
    makeCurrent();
    if (mouseEvent->button() == Qt::RightButton) {
        lastPos = mouseEvent->posF();

        mouseEvent->accept();
        return;
    }

    do {
        vec3 obj = unProject(mouseEvent->pos());

        QPair<Block *, Direction> face = world->getClosestFace(obj);
        Block * block = face.first;
        if (!block) break;

        bool accepted = block->clicked();

        if (accepted) {
            mouseEvent->accept();
            return;
        }

        Direction dir = face.second;

        vec3 offset = dirToOffset(dir);

        vec3 newPosition = block->position() + offset;

        if (world->blockAt(newPosition)) return;

        Block * newBlock = world->addBlock(newPosition, currentBlockType);

        if (!newBlock->validatePlacement(obj-(newPosition+vec3(0.5,0.5,0.5)), block)) {
            delete newBlock;
            return;
        }

        Q_ASSERT(newBlock->type() == currentBlockType);

        if (currentBlockType == Block::btWire) {
            newBlock->setPower(false);
        }
        else {
            Block * block; // TODO: remove shadowing

            for (int direction = dirFirstFlat; direction <= dirLastFlat; ++direction) {
                vec3 offset = dirToOffset(static_cast<Direction>(direction));

                vec3 pos = newPosition + offset;

                block = world->blockAt(pos);

                if (block && block->type() == Block::btWire)
                    block->setDirty();

                block = world->blockAt(pos + vec3(0,1,0));

                if (block && block->type() == Block::btWire)
                    block->setDirty();

                block = world->blockAt(pos + vec3(0,-1,0));

                if (block && block->type() == Block::btWire)
                    block->setDirty();
            }

            block = world->blockAt(newPosition + vec3(0,1,0));

            if (block && block->type() == Block::btWire)
                block->setDirty();

            block = world->blockAt(newPosition + vec3(0,-1,0));

            if (block && block->type() == Block::btWire)
                block->setDirty();
        }

        mouseEvent->accept();
        return;
    } while (0);

    QGLWidget::mousePressEvent(mouseEvent);
}

void GLWidget::mouseMoveEvent(QMouseEvent * mouseEvent)
{
    makeCurrent();
#ifdef DRAW_NORMAL
    do {
        vec3 obj = unProject(mouseEvent->pos());

        QPair<Block *, Direction> face = world->getClosestFace(obj);
        if (!face.first) break;

        BoundingBox box = face.first->boundingBox();

        normalstart = box.first + boxSize*0.5;
        boxSize = box.second;

        normalvec = dirToOffset(face.second);
        setDirty();
    } while (0);
#endif

    if (mouseEvent->buttons() & Qt::RightButton) {
        QPointF currentPos = mouseEvent->posF();
        QPointF delta = currentPos - lastPos;
        lastPos = currentPos;

        mouseDragged(delta);

        mouseEvent->accept();
        return;
    }

    QGLWidget::mouseMoveEvent(mouseEvent);
}

void GLWidget::mouseDragged(const QPointF &delta)
{
    mAngle.y += delta.x() * .5;
    mAngle.p += delta.y() * .5;
    setAngle(mAngle);
}

void GLWidget::mouseReleaseEvent(QMouseEvent * mouseEvent)
{
    if (mouseEvent->button() == Qt::RightButton) {


        mouseEvent->accept();
        return;
    }

    QGLWidget::mouseReleaseEvent(mouseEvent);
}

void GLWidget::wheelEvent(QWheelEvent * event)
{
    emit wheel(event->delta() > 0 ? -1 : 1);
}

void GLWidget::keyPressEvent(QKeyEvent * event)
{
    makeCurrent();
    switch(event->key())
    {
    case Qt::Key_Delete:
        vec3 obj = unProject(mapFromGlobal(QCursor::pos()));

        QPair<Block *, Direction> face = world->getClosestFace(obj);
        if (!face.first)
            break;

        delete face.first;

        world->setDirty();
        return;
        break;
    }
}

void GLWidget::focusOutEvent(QFocusEvent * event)
{
    switch(event->reason())
    {
    default:
        setFocus(Qt::OtherFocusReason);
    }
}

void GLWidget::initializeGL()
{
    // Set up the rendering context, define display lists etc.:
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_CULL_FACE);

    // fix outlines z-fighting with the quads
    glPolygonOffset(1, 1);
    glEnable(GL_POLYGON_OFFSET_FILL);
}

void GLWidget::resizeGL(int w, int h)
{
    w = w & ~1; h = h & ~1;
    // setup viewport, projection etc.:
    glViewport(0, 0, (GLint)w, (GLint)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w/2.0, w/2.0, h/2.0, -h/2.0, -1, 1);
    glScaled(16.0, 16.0, 1.0/16.0);
    matrix4<double> m(
        2,  0, -2, 0,
        1, -2,  1, 0,
        1,  2,  1, 0,
        0,  0,  0, 1
    );
    glMultMatrixd(m.transpose()[0]);
    //glMultTransposeMatrixd(m[0]);

}

void GLWidget::paintGL()
{
    mDirty = false;
    if (!world) return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world->draw();

#ifdef DRAW_NORMAL
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    glLineWidth(1);
    glColor3d(0, .5, 1);
    glBegin(GL_LINES);
    glhVertex(normalstart);
    glhVertex(normalstart+normalvec*16);
    glEnd();

    vec3 boxs = boxSize*1.2;
    vec3 boxp = normalstart-boxs*0.5;

    double x = boxp.x;
    double y = boxp.y;
    double z = boxp.z;
    double xs = boxs.x, ys = boxs.y, zs = boxs.z;

    glBegin(GL_LINE_LOOP);
    // top side
    glVertex3d(x     , y + ys, z     );
    glVertex3d(x + xs, y + ys, z     );
    glVertex3d(x + xs, y + ys, z + zs);
    glVertex3d(x     , y + ys, z + zs);
    glEnd();

    glBegin(GL_LINE_LOOP);
    // bottom side
    glVertex3d(x     , y     , z     );
    glVertex3d(x + xs, y     , z     );
    glVertex3d(x + xs, y     , z + zs);
    glVertex3d(x     , y     , z + zs);
    glEnd();

    glBegin(GL_LINE_LOOP);
    // east side
    glVertex3d(x     , y     , z     );
    glVertex3d(x + xs, y     , z     );
    glVertex3d(x + xs, y + ys, z     );
    glVertex3d(x     , y + ys, z     );
    glEnd();

    glBegin(GL_LINE_LOOP);
    // west side
    glVertex3d(x     , y     , z + zs);
    glVertex3d(x + xs, y     , z + zs);
    glVertex3d(x + xs, y + ys, z + zs);
    glVertex3d(x     , y + ys, z + zs);
    glEnd();

    glBegin(GL_LINE_LOOP);
    // north side
    glVertex3d(x     , y     , z     );
    glVertex3d(x     , y     , z + zs);
    glVertex3d(x     , y + ys, z + zs);
    glVertex3d(x     , y + ys, z     );
    glEnd();

    glBegin(GL_LINE_LOOP);
    // south side
    glVertex3d(x + xs, y     , z     );
    glVertex3d(x + xs, y     , z + zs);
    glVertex3d(x + xs, y + ys, z + zs);
    glVertex3d(x + xs, y + ys, z     );
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
#endif
}

void GLWidget::setDirty()
{
    if (mDirty)
        return;

    mDirty = true;

    QTimer::singleShot(0, this, SLOT(updateGL()));
}

void GLWidget::setAngle(angle<qreal> const & angle)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(angle.r, 0, 0, 1);
    glRotated(angle.p, 1, 0, 0);
    glRotated(angle.y, 0, 1, 0);
    setDirty();
}

void GLWidget::setWorld(World *world)
{
    if (this->world) {
        disconnect(this->world, SIGNAL(destroyed()), this, SLOT(resetWorld()));
        disconnect(this->world, SIGNAL(redrawNeeded()), this, SLOT(updateGL())); // causes qWarning, because it's invoked by the QObject dtor
    }

    this->world = world;

    if (world) {
        connect(world, SIGNAL(destroyed()), SLOT(resetWorld()));
        connect(world, SIGNAL(redrawNeeded()), SLOT(updateGL()));
    }
    updateGL();
}

void GLWidget::setCurrentBlockType(Block::BlockType newBlockType)
{
    currentBlockType = newBlockType;
}
