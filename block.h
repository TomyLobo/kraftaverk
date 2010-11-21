#ifndef BLOCK_H
#define BLOCK_H

#include "position.h"
#include "world.h"
#include "helper.h"
#include "displaylist.h"

#include <QObject>
#include <QList>
#include <QGraphicsItem>
#include <qnumeric.h>

class Block : public QObject
{
    Q_OBJECT
public:
    enum BlockType {
        btAir = 0,
        btStone = 1,
        btWire = 55,
        btDoor = 64,
        btLever = 69,
        btPressurePlate = 70,
        btTorch = 75,
        btButton = 77
    };

    virtual ~Block();
    virtual BlockType type() = 0;
    vec3 const & position() { return mPosition; }
    Direction attachment;
    virtual void clicked() {}
    virtual bool allowsAttachment() { return false; }
    void powerAllAround(vec3 const & centerPosition, bool on, Block * poweredFrom, Block * poweredVia);
    void setPower(bool on, Block * poweredFrom, Block * poweredVia);
    virtual void setPower(bool on) {}
    virtual bool validPowerSource(Block * poweredFrom, Block * poweredVia) { return false; }
    virtual void tick() {}

protected:
    explicit Block(vec3 const & position, World * parent = 0);

    World * world()
    {
        return static_cast<World*>(parent());
    }

    void boxhelper(vec3 const & position, vec3 const & size, QBrush const & brush);
    void boxhelper(double x, double y, double z, double xs, double ys, double zs, QBrush const & brush);

    virtual void drawGeometry() = 0;

    void setTicked(bool ticked);
    void setDirty()
    {
        mDirty = true;
        world()->setDirty();
    }
    void updateGeometry();

private:
    DisplayList dlist;
    vec3 mPosition;
    QSet<Block *> powerSources;
    bool mDirty;

public slots:
    void draw();
};

#endif // BLOCK_H
