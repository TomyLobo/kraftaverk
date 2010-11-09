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
    explicit Block(Position const & position, World * parent = 0);
    Position const & position() { return mPosition; }
    Direction attachment;
    virtual void clicked() {}

protected:
    QList<QGraphicsItem*> parts;
    Position mPosition;
    Position getCoords(Position const & position, double zOrderOverride = qInf())
    {
        return static_cast<World*>(parent())->getCoords(position, zOrderOverride);
    }
    QAbstractGraphicsShapeItem * dQuad(Position const & p1, Position const & p2, Position const & p3, Position const & p4);
    QGraphicsLineItem * dLine(Position const & p1, Position const & p2);
    QAbstractGraphicsShapeItem * dCircle(Position const & center, qreal radius);
    QList<QGraphicsItem *> boxhelper(double x, double y, double z, double xs, double ys, double zs, QBrush const & pen);

    virtual QList<QGraphicsItem *> getGeometry() = 0;

signals:

public slots:
    void updateGeometry();
};

#endif // BLOCK_H
