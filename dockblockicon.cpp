#include "dockblockicon.h"

#include "blockstone.h"
#include "blocktorch.h"
#include "blockbutton.h"
#include "blockwire.h"
#include "blockdoor.h"

#include "mathlib/matrix4.h"

DockBlockIcon::DockBlockIcon(QWidget *parent) :
    QGLWidget(parent),
    mActiveIcon(0)
{
    time.start();
    icons << new BlockStone(vec3());
    icons << new BlockTorch(vec3());
    icons.last()->attachment = dirDown;
    icons << new BlockWire(vec3());
    icons << new BlockButton(vec3());
    icons.last()->attachment = dirDown;
    icons << new BlockDoor(vec3());
    icons.last()->attachment = dirEast;

    startTimer(30);
}

DockBlockIcon::~DockBlockIcon()
{
    makeCurrent();
    qDeleteAll(icons);
}

void DockBlockIcon::timerEvent(QTimerEvent * event)
{
    updateGL();
}

void DockBlockIcon::mousePressEvent(QMouseEvent * mouseEvent)
{
    int clickedIndex = mouseEvent->pos().x() / height(); // TODO: move mousecoord2index to helper

    if (clickedIndex >= icons.size())
        return;

    if (mActiveIcon != clickedIndex) {
        mActiveIcon = clickedIndex;

        time.restart();
    }

    mouseEvent->accept();
}

void DockBlockIcon::drawIcon(int index, qreal yaw, bool highlighted, bool selected)
{
    int h = height(); // TODO: move to paintGL, add x/y

    glViewport(index * h, 0, h, h);

    if (yaw) {
        glMatrixMode(GL_MODELVIEW);
        glTranslatef(0.5,0.5,0.5);
        glRotatef(yaw, 0, 1, 0);
        glTranslatef(-0.5,-0.5,-0.5);
    }


    if (selected)
        glLineWidth(3);
    else
        glLineWidth(1);

    if (highlighted)
        glColor3f(0, 0.5, 1);
    else
        glColor3f(0.5, 0.75, 1);

    glDepthMask(GL_FALSE);

    glBegin(GL_LINE_LOOP);
    glVertex3d(0, 0, 0);
    glVertex3d(1, 0, 0);
    glVertex3d(1, 0, 1);
    glVertex3d(0, 0, 1);
    glEnd();

    glDepthMask(GL_TRUE);

    // draw the block
    icons[index]->draw();

    if (yaw) {
        glLoadIdentity();
    }
}

/*
void DockBlockIcon::setActiveIcon(int index) {

}
*/

void DockBlockIcon::initializeGL()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);

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

void DockBlockIcon::resizeGL(int w, int h)
{
    w=h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w/2.0, w/2.0, h/2.0, -h/2.0, -1, 1);
    glScaled(12.0, 12.0, 1.0/12.0);
    matrix4<double> m(
        2,  0, -2, 0,
        1, -2,  1, 0,
        1,  2,  1, 0,
        0,  0,  0, 1
    );
    glMultMatrixd(m.transpose()[0]);
}

void DockBlockIcon::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int nIcons = icons.size();
    qreal yaw = time.elapsed()/30.0;

    int highlightedIcon = mapFromGlobal(QCursor::pos()).x() / height(); // TODO: move mousecoord2index to helper

    for (int index = 0; index < nIcons; ++index) {
        bool selected = index == mActiveIcon;
        bool highlighted = index == highlightedIcon;
        drawIcon(index, selected ? yaw : 0, highlighted, selected);
    }
}
