#ifndef BLOCKTORCH_H
#define BLOCKTORCH_H

#include "block.h"
#include "helper.h"

class BlockTorch : public Block
{
    Q_OBJECT
private:
    bool mOn;
    bool nextOn;

public:
    explicit BlockTorch(vec3 const & position, World * parent = 0);
    virtual BlockType type();
    virtual void setPower(bool on);
    virtual bool validPowerSource(Block * poweredFrom, Block * poweredVia);
    virtual void tick();
    virtual BoundingBox boundingBox();
    virtual bool validatePlacement(const vec3 & where, Block * onWhat);

signals:

protected:
    virtual void drawGeometry();
};

#endif // BLOCKTORCH_H
