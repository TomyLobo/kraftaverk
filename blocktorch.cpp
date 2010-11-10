#include "blocktorch.h"

BlockTorch::BlockTorch(vec3 const & position, World *parent) :
    Block(position, parent),
    on(true)
{}

QList<QGraphicsItem *> BlockTorch::getGeometry()
{
    QList<QGraphicsItem *> ret;

    vec3 bottom, top;
    switch(attachment) {
        case dirEast:
        case dirWest:
        case dirNorth:
        case dirSouth:
            {
                vec3 offset = dirToOffset(attachment);
                bottom = vec3(position().x + 0.5 + offset.x * 0.5, position().y + 0.25, position().z + 0.5 + offset.z * 0.5);
                top    = vec3(position().x + 0.5 + offset.x * 0.3, position().y + 0.70, position().z + 0.5 + offset.z * 0.3);
            }
            break;

        default:
            bottom = vec3(position().x + 0.5, position().y + 0.0, position().z + 0.5);
            top    = vec3(position().x + 0.5, position().y + 0.5, position().z + 0.5);
            break;
    }

    QGraphicsLineItem * handle = dLine(bottom, top);
    ret << handle;

    QAbstractGraphicsShapeItem * tip = dCircle(top, 5);
    ret << tip;

    handle->setPen(QPen(QBrush(QColor(128, 96, 0)), 3, Qt::SolidLine, Qt::RoundCap));

    tip->setPen(QPen(Qt::NoPen));
    tip->setBrush(QBrush(on ? QColor(255, 0, 0) : QColor(64, 32, 0)));

    return ret;
}
