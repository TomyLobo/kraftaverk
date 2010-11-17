#ifndef BLOCKWIRE_H
#define BLOCKWIRE_H

#include "block.h"

class BlockWire : public Block
{
    Q_OBJECT
private:
    int lastCharge;
    QList<Block *> neighboringWires(bool devicesToo = false);
    QList<BlockWire *> unchargeConnectedWires();

public:
    explicit BlockWire(vec3 const & position, World * parent = 0);
    virtual BlockType type();

signals:

public slots:
    virtual QList<QGraphicsItem *> getGeometry();
};

#endif // BLOCKWIRE_H
