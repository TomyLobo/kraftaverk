#include "block.h"

#include "world.h"

#include <QTimer>

Block::Block(vec3 const & position, World *parent) :
    QObject(parent),
    mPosition(position)
{
    parent->insertBlock(this);
}

void Block::updateGeometry()
{
    QGraphicsScene * scene = world()->scene();
    foreach(QGraphicsItem * part, parts) {
        scene->removeItem(part);
    }

    QList<QGraphicsItem *> newParts = getGeometry();
    parts = newParts;

    foreach(QGraphicsItem * part, parts) {
        scene->addItem(part);
    }
}

void Block::redraw()
{
    updateGeometry(); // TEMP!
    //QGraphicsScene * scene = world()->scene();
}

QAbstractGraphicsShapeItem * Block::dQuad(vec3 const & p1, vec3 const & p2, vec3 const & p3, vec3 const & p4, double zOrderOverride)
{
    // projection
    vec3 p1p = getCoords(p1, zOrderOverride);
    vec3 p2p = getCoords(p2, zOrderOverride);
    vec3 p3p = getCoords(p3, zOrderOverride);
    vec3 p4p = getCoords(p4, zOrderOverride);

    // generate polygon structure
    QPolygonF polygon;

    polygon
        << QPointF(p1p.x, p1p.y)
        << QPointF(p2p.x, p2p.y)
        << QPointF(p3p.x, p3p.y)
        << QPointF(p4p.x, p4p.y);

    // generate polygon shape from it
    QAbstractGraphicsShapeItem * ret = new QGraphicsPolygonItem(polygon);

    // set z value for z ordering
    ret->setZValue((p1p.z + p2p.z + p3p.z + p4p.z) / 4);

    // reduce line size so it doesn't look crap with AA
    ret->setPen(QPen(QBrush(QColor(0,0,0)),0.5));

    return ret;
}

QAbstractGraphicsShapeItem * Block::dCircle(vec3 const & center, qreal radius, double zOrderOverride)
{
    vec3 centerp = getCoords(center, zOrderOverride);

    QAbstractGraphicsShapeItem * ret = new QGraphicsEllipseItem(centerp.x-radius, centerp.y-radius, radius*2, radius*2);
    ret->setZValue(centerp.z);
    return ret;
}

QGraphicsLineItem * Block::dLine(vec3 const & p1, vec3 const & p2, double zOrderOverride)
{
    vec3 p1p = getCoords(p1, zOrderOverride);
    vec3 p2p = getCoords(p2, zOrderOverride);

    QGraphicsLineItem * ret = new QGraphicsLineItem(p1p.x, p1p.y, p2p.x, p2p.y);
    ret->setZValue((p1p.z + p2p.z) / 2);
    return ret;
}

QList<QGraphicsItem *> Block::boxhelper(double x, double y, double z, double xs, double ys, double zs, QBrush const & pen, double zOrderOverride)
{
    QBrush brushTop = pen;

    QBrush brushEW = pen;
    brushEW.setColor( pen.color().darker(120));

    QBrush brushNS = pen;
    brushNS.setColor( pen.color().darker(150));

    QList<QGraphicsItem *> ret;
    QAbstractGraphicsShapeItem * top, * EW, * NS;

    ret << (top = dQuad(
        vec3(x     , y + ys, z     ),
        vec3(x + xs, y + ys, z     ),
        vec3(x + xs, y + ys, z + zs),
        vec3(x     , y + ys, z + zs),
        zOrderOverride
    ));

    if (world()->sideVisible(dirEast)) {
        // east side
        ret << (EW = dQuad(
            vec3(x     , y     , z     ),
            vec3(x + xs, y     , z     ),
            vec3(x + xs, y + ys, z     ),
            vec3(x     , y + ys, z     ),
            zOrderOverride
        ));
    }
    else {
        // west side
        ret << (EW = dQuad(
            vec3(x     , y     , z + zs),
            vec3(x + xs, y     , z + zs),
            vec3(x + xs, y + ys, z + zs),
            vec3(x     , y + ys, z + zs),
            zOrderOverride
        ));
    }

    if (world()->sideVisible(dirNorth)) {
        // north side
        ret << (NS = dQuad(
            vec3(x     , y     , z     ),
            vec3(x     , y     , z + zs),
            vec3(x     , y + ys, z + zs),
            vec3(x     , y + ys, z     ),
            zOrderOverride
        ));
    }
    else {
        // south side
        ret << (NS = dQuad(
            vec3(x + xs, y     , z     ),
            vec3(x + xs, y     , z + zs),
            vec3(x + xs, y + ys, z + zs),
            vec3(x + xs, y + ys, z     ),
            zOrderOverride
        ));
    }

    top->setBrush(brushTop);
    EW->setBrush(brushEW);
    NS->setBrush(brushNS);

    return ret;
}
