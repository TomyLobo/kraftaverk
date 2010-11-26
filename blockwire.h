#ifndef BLOCKWIRE_H
#define BLOCKWIRE_H

#include "block.h"

#include <QSet>

class BlockWire : public Block
{
    Q_OBJECT
private:
    int lastCharge;

    QList<Block *> neighboringWires(bool devicesToo = false);
    QList<BlockWire *> unchargeConnectedWires();
    virtual void setPower(bool on);
    virtual bool validPowerSource(Block * poweredFrom, Block * poweredVia);
    void setCharge(int charge);
    virtual BoundingBox boundingBox();

public:
    explicit BlockWire(vec3 const & position, World * parent = 0);
    virtual BlockType type();

signals:

public slots:
    virtual void drawGeometry();
};

#endif // BLOCKWIRE_H
