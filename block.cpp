#include "block.h"

#include "world.h"

#include <QTimer>

Block::Block(Position const & position, World *parent) :
    QObject(parent),
    mPosition(position)
{
    parent->insertBlock(this);
    //QTimer::singleShot(0, this, SLOT(updateGeometry()));
}

void Block::updateGeometry()
{
    QGraphicsScene * scene = static_cast<QGraphicsScene *>(parent()->parent());
    foreach(QGraphicsItem * part, parts) {
        scene->removeItem(part);
    }

    QList<QGraphicsItem *> newParts = getGeometry();
    parts = newParts;

    foreach(QGraphicsItem * part, parts) {
        scene->addItem(part);
    }
}

QAbstractGraphicsShapeItem * Block::dQuad(Position const & p1, Position const & p2, Position const & p3, Position const & p4)
{
    Position p1p = getCoords(p1);
    Position p2p = getCoords(p2);
    Position p3p = getCoords(p3);
    Position p4p = getCoords(p4);

    QPolygonF polygon;

    polygon
        << QPointF(p1p.x, p1p.y)
        << QPointF(p2p.x, p2p.y)
        << QPointF(p3p.x, p3p.y)
        << QPointF(p4p.x, p4p.y);

    QAbstractGraphicsShapeItem * ret = new QGraphicsPolygonItem(polygon);
    ret->setZValue((p1p.z + p2p.z + p3p.z + p4p.z) / 4);
    return ret;
}

QAbstractGraphicsShapeItem * Block::dCircle(Position const & center, qreal radius)
{
    Position centerp = getCoords(center);

    QAbstractGraphicsShapeItem * ret = new QGraphicsEllipseItem(centerp.x-radius, centerp.y-radius, radius*2, radius*2);
    ret->setZValue(centerp.z);
    return ret;
}

QGraphicsLineItem * Block::dLine(Position const & p1, Position const & p2)
{
    Position p1p = getCoords(p1);
    Position p2p = getCoords(p2);

    QGraphicsLineItem * ret = new QGraphicsLineItem(p1p.x, p1p.y, p2p.x, p2p.y);
    ret->setZValue((p1p.z + p2p.z) / 2);
    return ret;
}

QList<QGraphicsItem *> Block::boxhelper(double x, double y, double z, double xs, double ys, double zs, QBrush const & pen)
{
    QBrush brushTop = pen;

    QBrush brushEW = pen;
    brushEW.setColor( pen.color().darker(120));

    QBrush brushNS = pen;
    brushNS.setColor( pen.color().darker(150));

    QList<QGraphicsItem *> ret;
    QAbstractGraphicsShapeItem * top, * EW, * NS;

    ret << (top = dQuad(
        Position(x     , y + ys, z     ),
        Position(x + xs, y + ys, z     ),
        Position(x + xs, y + ys, z + zs),
        Position(x     , y + ys, z + zs)
    ));

    //if (sideVisible(dirEast)) {
    if (false) {
        // east side
        ret << (EW = dQuad(
            Position(x     , y     , z     ),
            Position(x + xs, y     , z     ),
            Position(x + xs, y + ys, z     ),
            Position(x     , y + ys, z     )
        ));
    }
    else {
        // west side
        ret << (EW = dQuad(
            Position(x     , y     , z + zs),
            Position(x + xs, y     , z + zs),
            Position(x + xs, y + ys, z + zs),
            Position(x     , y + ys, z + zs)
        ));
    }

    //if (sideVisible(dirNorth)) {
    if (false) {
        // north side
        ret << (NS = dQuad(
            Position(x     , y     , z     ),
            Position(x     , y     , z + zs),
            Position(x     , y + ys, z + zs),
            Position(x     , y + ys, z     )
        ));
    }
    else {
        // south side
        ret << (NS = dQuad(
            Position(x + xs, y     , z     ),
            Position(x + xs, y     , z + zs),
            Position(x + xs, y + ys, z + zs),
            Position(x + xs, y + ys, z     )
        ));
    }

    top->setBrush(brushTop);
    EW->setBrush(brushEW);
    NS->setBrush(brushNS);

    return ret;
}
