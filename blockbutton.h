#ifndef BLOCKBUTTON_H
#define BLOCKBUTTON_H

#include "block.h"

class BlockButton : public Block
{
    Q_OBJECT
private:
    bool on;

public:
    explicit BlockButton(Position const & position, World * parent = 0);
    void clicked();

signals:

private slots:
    //void clicked();

public slots:
    virtual QList<QGraphicsItem *> getGeometry();
};

#endif // BLOCKBUTTON_H