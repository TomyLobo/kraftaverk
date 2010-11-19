#include "blocktorch.h"

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

QList<QGraphicsItem *> BlockTorch::getGeometry()
{
    QList<QGraphicsItem *> ret;

    vec3 bottom, top;
    vec3 bottom2, top2;

    vec3 offset = dirToOffset(attachment);
    bottom = position() + vec3(0.5, 0.25, 0.5) + offset * vec3(0.5, 0.25, 0.5);
    top    = position() + vec3(0.5, 0.70, 0.5) + offset * vec3(0.3, 0.20, 0.3);

    qreal centerz = getCoords(position() + vec3(0.5, 0.5, 0.5)).z;

    QGraphicsLineItem * handle = dLine(bottom, top, centerz);
    ret << handle;

    QAbstractGraphicsShapeItem * tip = dCircle(top, 5, centerz);
    ret << tip;

    handle->setPen(QPen(QBrush(QColor(128, 96, 0)), 3, Qt::SolidLine, Qt::RoundCap));

    tip->setPen(QPen(Qt::NoPen));
    tip->setBrush(QBrush(mOn ? QColor(255, 0, 0) : QColor(64, 32, 0)));

    return ret;
}
bool BlockTorch::validPowerSource(Block * poweredFrom, Block * poweredVia)
{
    return poweredVia && poweredVia->position() == position() + dirToOffset(attachment);
}

void BlockTorch::setPower(bool on)
{
    if (on != nextOn) return;

    nextOn = !on;

    //tick(); // TEMP!
}

void BlockTorch::tick()
{
    if (mOn == nextOn) return;

    mOn = nextOn;
    updateGeometry(); // TODO: optimize

    // maybe merge with blockwire into a helper function
    vec3 abovePosition = position() + vec3(0, 1, 0);

    Block * above = world()->blockAt(abovePosition);

    for (int direction = dirFirstAll; direction != dirLastAll; ++direction) {
        vec3 offset = dirToOffset(static_cast<Direction>(direction));

        vec3 pos1 = position() + offset;
        vec3 pos2 = abovePosition + offset;

        Block * block1 = world()->blockAt(pos1);
        Block * block2 = world()->blockAt(pos2);

        if (block1) block1->setPower(mOn, this, 0);
        if (block2) block2->setPower(mOn, this, above);
    }
}
