#include "block.h"
#include "world.h"

#include <QTimer>

Block::Block(vec3 const & position, World *parent) :
    QObject(parent),
    mPosition(position),
    mDirty(false)
{
    if (parent)
        parent->insertBlock(this);
    setDirty();
}

Block::~Block()
{
    if (world())
        world()->removeBlock(this);
    setTicked(false);
}

World * Block::world()
{
    return static_cast<World*>(parent());
}

void Block::updateGeometry()
{
    dlist.begin();
    drawGeometry();
    dlist.end();
    mDirty = false;
}

void Block::draw()
{
    if (mDirty)
        updateGeometry();

    dlist.call();
}

void Block::boxhelper(BoundingBox const & box, QBrush const & brush)
{
    boxhelper(box.first, box.second, brush);
}

void Block::boxhelper(vec3 const & position, vec3 const & size, QBrush const & brush)
{
    boxhelper(position.x, position.y, position.z, size.x, size.y, size.z, brush);
}

void Block::boxhelper(double x, double y, double z, double xs, double ys, double zs, QBrush const & brush)
{
    QBrush brushTop = brush;

    QBrush brushEW = brush;
    brushEW.setColor( brush.color().darker(120));

    QBrush brushNS = brush;
    brushNS.setColor( brush.color().darker(150));

    glBegin(GL_QUADS);
    glhApplyBrush(brushTop);
    // top side
    glVertex3d(x     , y + ys, z     );
    glVertex3d(x     , y + ys, z + zs);
    glVertex3d(x + xs, y + ys, z + zs);
    glVertex3d(x + xs, y + ys, z     );

    // bottom side
    glVertex3d(x     , y     , z     );
    glVertex3d(x + xs, y     , z     );
    glVertex3d(x + xs, y     , z + zs);
    glVertex3d(x     , y     , z + zs);

    glhApplyBrush(brushEW);
    // east side
    glVertex3d(x     , y     , z     );
    glVertex3d(x     , y + ys, z     );
    glVertex3d(x + xs, y + ys, z     );
    glVertex3d(x + xs, y     , z     );

    // west side
    glVertex3d(x     , y     , z + zs);
    glVertex3d(x + xs, y     , z + zs);
    glVertex3d(x + xs, y + ys, z + zs);
    glVertex3d(x     , y + ys, z + zs);

    glhApplyBrush(brushNS);
    // north side
    glVertex3d(x     , y     , z     );
    glVertex3d(x     , y     , z + zs);
    glVertex3d(x     , y + ys, z + zs);
    glVertex3d(x     , y + ys, z     );

    // south side
    glVertex3d(x + xs, y     , z     );
    glVertex3d(x + xs, y + ys, z     );
    glVertex3d(x + xs, y + ys, z + zs);
    glVertex3d(x + xs, y     , z + zs);

    glEnd();

    QPen outlinePen;
    outlinePen.setWidthF(1);

    glhApplyPen(outlinePen);
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
}

void Block::setTicked(bool ticked)
{
    if (world())
        world()->setTicked(this, ticked);
}

void Block::setPower(bool on, Block * poweredFrom, Block * poweredVia)
{
    if (!validPowerSource(poweredFrom, poweredVia)) return;

    bool prevEmpty = powerSources.isEmpty();
    if (on) {
        powerSources += poweredFrom;
    }
    else {
        powerSources -= poweredFrom;
    }

    if (powerSources.isEmpty() == prevEmpty) return;

    setPower(!powerSources.isEmpty());
}

void Block::powerAllAround(vec3 const & centerPosition, bool on, Block * poweredFrom, Block * poweredVia)
{
    for (int direction = dirFirstAll; direction <= dirLastAll; ++direction) {
        vec3 offset = dirToOffset(static_cast<Direction>(direction));

        vec3 pos = centerPosition + offset;

        Block * block = world()->blockAt(pos);

        if (block) block->setPower(on, poweredFrom, poweredVia);
    }
}

void Block::setDirty()
{
    mDirty = true;
    if (world())
        world()->setDirty();
}
