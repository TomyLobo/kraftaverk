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
    boxhelper(position(), vec3(1.0, 1.0, 1.0), QBrush(QColor(192, 192, 192)));
}
