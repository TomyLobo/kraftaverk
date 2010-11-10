#ifndef BLOCKWIRE_H
#define BLOCKWIRE_H

#include "block.h"

class BlockWire : public Block
{
    Q_OBJECT
public:
    explicit BlockWire(vec3 const & position, World * parent = 0);

signals:

public slots:
    virtual QList<QGraphicsItem *> getGeometry();
};

#endif // BLOCKWIRE_H
