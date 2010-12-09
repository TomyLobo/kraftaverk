#include "blockpressureplate.h"

#include "world.h"

BlockPressurePlate::BlockPressurePlate(vec3 const & position, World *parent) :
    BlockButton(position, parent),
    mOn(false)
{
    attachment = dirDown;
}

Block::BlockType BlockPressurePlate::type()
{
    return btPressurePlate;
}

BoundingBox BlockPressurePlate::boundingBox()
{
    qreal buttonHeight = on() ? 0.05 : 0.1;
    qreal buttonWidth = 0.9;

    vec3 offset = dirToOffset(attachment);

    return qMakePair(
        vec3(
            position().x + (offset.x ? (1 - buttonHeight) * (offset.x * 0.5 + 0.5) : (1-buttonWidth)*0.5),
            position().y + (offset.y ? (1 - buttonHeight) * (offset.y * 0.5 + 0.5) : (1-buttonWidth)*0.5),
            position().z + (offset.z ? (1 - buttonHeight) * (offset.z * 0.5 + 0.5) : (1-buttonWidth)*0.5)
        ),
        vec3(
            offset.x ? buttonHeight : buttonWidth,
            offset.y ? buttonHeight : buttonWidth,
            offset.z ? buttonHeight : buttonWidth
        )
    );
}

bool BlockPressurePlate::validatePlacement(const vec3 & where, Block * onWhat)
{
    attachment = dirDown;

    Block * below = world()->blockAt(position()+vec3(0, -1, 0));
    return below && below->allowsAttachment();
}
