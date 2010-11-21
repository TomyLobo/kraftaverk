#include "glwidget.h"
#include "block.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::DirectRendering), parent),
    world(0)
{
}

void GLWidget::mousePressEvent(QMouseEvent * mouseEvent)
{
    if (mouseEvent->button() == Qt::RightButton) {
        lastPos = mouseEvent->posF();

        mouseEvent->accept();
        return;
    }

    do {
        GLdouble projection[16];
        glGetDoublev(GL_PROJECTION_MATRIX, projection);

        GLdouble modelView[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, modelView);

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        double winX = mouseEvent->pos().x();
        double winY = viewport[3] - mouseEvent->pos().y();

        GLfloat winZ;
        glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

        double objX, objY, objZ;
        gluUnProject(winX, winY, winZ, modelView, projection, viewport, &objX, &objY, &objZ);

        vec3 obj(floor(objX), floor(objY), floor(objZ));
        Block *block = world->blockAt(obj);
        if (!block) break;

        block->clicked();

        mouseEvent->accept();
        return;
    } while (0);

    QGLWidget::mousePressEvent(mouseEvent);
}

void GLWidget::mouseMoveEvent(QMouseEvent * mouseEvent)
{
    if (mouseEvent->buttons() & Qt::RightButton) {
        QPointF currentPos = mouseEvent->posF();
        QPointF delta = currentPos - lastPos;
        lastPos = currentPos;

        emit mouseDragged(delta);

        mouseEvent->accept();
        return;
    }

    QGLWidget::mouseMoveEvent(mouseEvent);
}

void GLWidget::mouseReleaseEvent(QMouseEvent * mouseEvent)
{
    if (mouseEvent->button() == Qt::RightButton) {


        mouseEvent->accept();
        return;
    }

    QGLWidget::mouseReleaseEvent(mouseEvent);
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
#if 0
    glScaled(32.0/w, -32.0/h, 1/16.0);
#else
    glOrtho(-w/2.0, w/2.0, h/2.0, -h/2.0, -1, 1);
    glScaled(16.0, 16.0, 1.0/16.0);
#endif
#if 0
    double dist = sqrt(1 / 3.0);
    gluLookAt(dist, dist, dist,  /* position of camera */
              0.0,  0.0,  0.0,   /* where camera is pointing at */
              0.0,  1.0,  0.0);
#else
    matrix4<double> m(
        2,  0, -2, 0,
        1, -2,  1, 0,
        1,  2,  1, 0,
        0,  0,  0, 1
    );
    m *= 16;
    glMultMatrixd(m.transpose()[0]);
    //glMultTransposeMatrixd(m[0]);
#endif

}

void GLWidget::paintGL()
{
    if (!world) return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world->draw();
}

void GLWidget::setYaw(qreal yaw)
{
    qreal cy = cos(yaw);
    qreal sy = sin(yaw);

    matrix4<qreal> modelView = matrix4<qreal>(
        cy, 0, -sy, 0,
         0, 1,   0, 0,
        sy, 0,  cy, 0,
         0, 0,   0, 1
    );
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(modelView[0]); // change
    updateGL();
}

void GLWidget::setWorld(World *world)
{
    this->world = world;
    connect(world, SIGNAL(redrawNeeded()), SLOT(updateGL()));
    updateGL();
}
