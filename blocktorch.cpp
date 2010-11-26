#include "blocktorch.h"
#include "world.h"

#include "helper.h"

BlockTorch::BlockTorch(vec3 const & position, World *parent) :
    Block(position, parent),
    mOn(false),
    nextOn(true)
{
    setTicked(true);
}

Block::BlockType BlockTorch::type()
{
    return btTorch;
}

void BlockTorch::drawGeometry()
{
    vec3 bottom, top;

    vec3 offset = dirToOffset(attachment);
    bottom = position() + vec3(0.5, 0.25, 0.5) + offset * vec3(0.5, 0.25, 0.5);
    top    = position() + vec3(0.5, 0.70, 0.5) + offset * vec3(0.3, 0.20, 0.3);

    vec3 center = position() + vec3(0.5, 0.5, 0.5);

    static QPen handlePen(QBrush(QColor(128, 96, 0)), 3, Qt::SolidLine, Qt::RoundCap);

    glLineWidth(3);
    glColor3d(0.5, 0.375, 0);
    glBegin(GL_LINES);
        glhVertex(bottom);
        glhVertex(top);
    glEnd();

    if (mOn)
        glColor3d(1, 0, 0);
    else
        glColor3d(0.25, 0.125, 0);

    glPointSize(10);
    glBegin(GL_POINTS);
        glhVertex(top);
    glEnd();
}

bool BlockTorch::validPowerSource(Block * poweredFrom, Block * poweredVia)
{
    return poweredVia && poweredVia->position() == position() + dirToOffset(attachment);
}

void BlockTorch::setPower(bool on)
{
    if (on != nextOn) return;

    nextOn = !on;
}

void BlockTorch::tick()
{
    if (mOn == nextOn) return;

    mOn = nextOn;
    setDirty();

    // maybe merge with blockwire into a helper function
    vec3 abovePosition = position() + vec3(0, 1, 0);

    Block * above = world()->blockAt(abovePosition);

    powerAllAround(position(), mOn, this, 0);
    if (above)
        powerAllAround(abovePosition, mOn, this, above);
}

BoundingBox BlockTorch::boundingBox()
{
    // TODO: change!
    return qMakePair(position(), vec3(1.0, 1.0, 1.0));
}
