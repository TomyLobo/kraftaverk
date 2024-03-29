#ifndef BLOCKDOOR_H
#define BLOCKDOOR_H

#include "block.h"

class BlockDoor : public Block
{
    Q_OBJECT
private:
    bool on;
public:
    explicit BlockDoor(vec3 const & position, World * parent = 0);
    virtual BlockType type();
    virtual void setPower(bool on);
    virtual bool validPowerSource(Block * poweredFrom, Block * poweredVia);
    virtual BoundingBox boundingBox();
    virtual bool validatePlacement(const vec3 & where, Block * onWhat);

signals:

protected:
    virtual void drawGeometry();
};

#endif // BLOCKDOOR_H
