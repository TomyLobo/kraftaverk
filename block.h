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
    explicit Block(vec3 const & position, World * parent = 0);
    vec3 const & position() { return mPosition; }
    Direction attachment;
    virtual void clicked() {}

protected:
    QList<QGraphicsItem*> parts;
    vec3 mPosition;

    World * world()
    {
        return static_cast<World*>(parent());
    }

    vec3 getCoords(vec3 const & position, double zOrderOverride = qInf())
    {
        return world()->getCoords(position, zOrderOverride);
    }
    QAbstractGraphicsShapeItem * dQuad(vec3 const & p1, vec3 const & p2, vec3 const & p3, vec3 const & p4);
    QGraphicsLineItem * dLine(vec3 const & p1, vec3 const & p2);
    QAbstractGraphicsShapeItem * dCircle(vec3 const & center, qreal radius);
    QList<QGraphicsItem *> boxhelper(double x, double y, double z, double xs, double ys, double zs, QBrush const & pen);

    virtual QList<QGraphicsItem *> getGeometry() = 0;

signals:

public slots:
    void updateGeometry();
};

#endif // BLOCK_H
