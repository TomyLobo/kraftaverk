#ifndef BLOCKTORCH_H
#define BLOCKTORCH_H

#include "block.h"
#include "helper.h"

class BlockTorch : public Block
{
    Q_OBJECT
private:
    bool on;

public:
    explicit BlockTorch(Position const & position, World * parent = 0);

signals:

public slots:
    virtual QList<QGraphicsItem *> getGeometry();
};

#endif // BLOCKTORCH_H
