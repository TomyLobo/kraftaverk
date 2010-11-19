#ifndef BLOCK_H
#define BLOCK_H

#include "position.h"
#include "world.h"
#include "helper.h"

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

    vec3 getCoords(vec3 const & position, double zOrderOverride = qInf())
    {
        return world()->getCoords(position, zOrderOverride);
    }
    QAbstractGraphicsShapeItem * dQuad(vec3 const & p1, vec3 const & p2, vec3 const & p3, vec3 const & p4, double zOrderOverride = qInf());
    QGraphicsLineItem * dLine(vec3 const & p1, vec3 const & p2, double zOrderOverride = qInf());
    QAbstractGraphicsShapeItem * dCircle(vec3 const & center, qreal radius, double zOrderOverride = qInf());
    QList<QGraphicsItem *> boxhelper(double x, double y, double z, double xs, double ys, double zs, QBrush const & pen, double zOrderOverride = qInf());

    virtual QList<QGraphicsItem *> getGeometry() = 0;

    void setTicked(bool ticked);

private:
    QList<QGraphicsItem*> parts;
    vec3 mPosition;
    QSet<Block *> powerSources;

signals:

public slots:
    void redraw();
    void updateGeometry();
};

#endif // BLOCK_H
