#ifndef BLOCKSTONE_H
#define BLOCKSTONE_H

#include "block.h"

class BlockStone : public Block
{
    Q_OBJECT
public:
    explicit BlockStone(vec3 const & position, World * parent = 0);
    virtual BlockType type();
    virtual bool allowsAttachment() { return true; }
    virtual BoundingBox boundingBox();

signals:

protected:
    virtual void drawGeometry();
};

#endif // BLOCKSTONE_H
