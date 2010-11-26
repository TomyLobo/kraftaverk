#include "blockstone.h"

BlockStone::BlockStone(vec3 const & position, World *parent) :
    Block(position, parent)
{
}

Block::BlockType BlockStone::type()
{
    return btStone;
}

void BlockStone::drawGeometry()
{
    boxhelper(boundingBox(), QBrush(QColor(192, 192, 192)));
}

BoundingBox BlockStone::boundingBox()
{
    return qMakePair(position(), vec3(1.0, 1.0, 1.0));
}
