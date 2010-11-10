#ifndef BLOCKSTONE_H
#define BLOCKSTONE_H

#include "block.h"

class BlockStone : public Block
{
    Q_OBJECT
public:
    explicit BlockStone(vec3 const & position, World * parent = 0);

signals:

public slots:
    virtual QList<QGraphicsItem *> getGeometry();
};

#endif // BLOCKSTONE_H
