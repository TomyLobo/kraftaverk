#include "blockstone.h"

BlockStone::BlockStone(Position const & position, World *parent) :
    Block(position, parent)
{
}

QList<QGraphicsItem *> BlockStone::getGeometry()
{
    QList<QGraphicsItem *> ret;
    ret << boxhelper(position().x, position().y, position().z, 1.0, 1.0, 1.0, QBrush(QColor(192, 192, 192)));

    return ret;
}
