#include "blockwire.h"

BlockWire::BlockWire(vec3 const & position, World *parent) :
    Block(position, parent)
{
}

QList<QGraphicsItem *> BlockWire::getGeometry()
{
    QList<QGraphicsItem *> ret;
    ret << boxhelper(position().x, position().y, position().z, 1.0, .1, 1.0, QBrush(QColor(192, 0, 0)));

    return ret;
}
