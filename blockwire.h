#ifndef BLOCKWIRE_H
#define BLOCKWIRE_H

#include "block.h"

#include <QSet>

class BlockWire : public Block
{
    Q_OBJECT
private:
    int lastCharge;
    QSet<Block *> powerSources;

    QList<Block *> neighboringWires(bool devicesToo = false);
    QList<BlockWire *> unchargeConnectedWires();
    virtual void setPower(bool on, Block * poweredFrom);
    void setCharge(int charge);

public:
    explicit BlockWire(vec3 const & position, World * parent = 0);
    virtual BlockType type();

signals:

public slots:
    virtual QList<QGraphicsItem *> getGeometry();
};

#endif // BLOCKWIRE_H
