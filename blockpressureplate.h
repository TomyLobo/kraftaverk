#ifndef BLOCKPRESSUREPLATE_H
#define BLOCKPRESSUREPLATE_H

#include "blockbutton.h"

class BlockPressurePlate : public BlockButton
{
    Q_OBJECT
private:
    bool mOn;

public:
    explicit BlockPressurePlate(vec3 const & position, World * parent = 0);
    virtual BlockType type();
    virtual BoundingBox boundingBox();
    virtual bool validatePlacement(const vec3 & where, Block * onWhat);

};

#endif // BLOCKPRESSUREPLATE_H
