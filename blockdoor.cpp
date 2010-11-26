#include "blockdoor.h"

BlockDoor::BlockDoor(vec3 const & position, World *parent) :
    Block(position, parent),
    on(false)
{
}

Block::BlockType BlockDoor::type()
{
    return btDoor;
}

BoundingBox BlockDoor::boundingBox()
{
    vec3 offset;
    if (on) {
        offset = dirToOffset(dirAdd(attachment, daClockwise));
    }
    else {
        offset = dirToOffset(attachment);
    }

    double x = position().x;
    double y = position().y;
    double z = position().z;

    double xo = offset.x;
    double yo = offset.y;
    double zo = offset.z;

    return qMakePair(
        vec3(
            x + (xo ? 0.8 * (xo * 0.5 + 0.5) : 0),
            y + (yo ? 0.8 * (yo * 0.5 + 0.5) : 0),
            z + (zo ? 0.8 * (zo * 0.5 + 0.5) : 0)
        ),
        vec3(
            xo ? 0.2 : 1,
            yo ? 0.2 : 1,
            zo ? 0.2 : 1
        )
    );
}
void BlockDoor::drawGeometry()
{
    boxhelper(boundingBox(), QBrush(QColor(255, 204, 0)));
}

bool BlockDoor::validPowerSource(Block * poweredFrom, Block * poweredVia)
{
    return true;
}

void BlockDoor::setPower(bool on)
{
    if (on == this->on) return;

    this->on = on;

    setDirty();
}
