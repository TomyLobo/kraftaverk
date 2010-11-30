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
    virtual bool clicked();
    bool on();
    void setOn(bool on);
    virtual BoundingBox boundingBox();
    virtual bool validatePlacement(const vec3 & where, Block * onWhat);

signals:

protected:
    virtual void drawGeometry();
};

#endif // BLOCKBUTTON_H
