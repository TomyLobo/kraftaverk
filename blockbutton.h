#ifndef BLOCKBUTTON_H
#define BLOCKBUTTON_H

#include "block.h"

class BlockButton : public Block
{
    Q_OBJECT
private:
    bool mOn;

public:
    explicit BlockButton(vec3 const & position, World * parent = 0);
    virtual BlockType type();
    void clicked();
    bool on();
    void setOn(bool on);
    virtual BoundingBox boundingBox();

signals:

protected:
    virtual void drawGeometry();
};

#endif // BLOCKBUTTON_H
