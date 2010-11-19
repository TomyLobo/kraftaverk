#include "blocktorch.h"

BlockTorch::BlockTorch(vec3 const & position, World *parent) :
    Block(position, parent),
    on(true)
{}

Block::BlockType BlockTorch::type()
{
    return btTorch;
}

QList<QGraphicsItem *> BlockTorch::getGeometry()
{
    QList<QGraphicsItem *> ret;

    vec3 bottom, top;
    vec3 bottom2, top2;

    vec3 offset = dirToOffset(attachment);
    bottom = position() + vec3(0.5, 0.25, 0.5) + offset * vec3(0.5, 0.25, 0.5);
    top    = position() + vec3(0.5, 0.70, 0.5) + offset * vec3(0.3, 0.20, 0.3);

    qreal centerz = getCoords(position() + vec3(0.5, 0.5, 0.5)).z;

    QGraphicsLineItem * handle = dLine(bottom, top, centerz);
    ret << handle;

    QAbstractGraphicsShapeItem * tip = dCircle(top, 5, centerz);
    ret << tip;

    handle->setPen(QPen(QBrush(QColor(128, 96, 0)), 3, Qt::SolidLine, Qt::RoundCap));

    tip->setPen(QPen(Qt::NoPen));
    tip->setBrush(QBrush(on ? QColor(255, 0, 0) : QColor(64, 32, 0)));

    return ret;
}

void BlockTorch::setPower(bool on, Block * poweredFrom)
{
