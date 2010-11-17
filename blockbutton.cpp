#include "blockbutton.h"

BlockButton::BlockButton(vec3 const & position, World *parent) :
    Block(position, parent),
    on(false)
{
}

Block::BlockType BlockButton::type()
{
    return btButton;
}

QList<QGraphicsItem *> BlockButton::getGeometry()
{
    QList<QGraphicsItem *> ret;

    qreal buttonHeight = on ? 0.1 : 0.25;
    qreal buttonWidth = 0.5;

    vec3 offset = dirToOffset(attachment);

    ret << boxhelper(
        position().x + (offset.x ? (1 - buttonHeight) * (offset.x * 0.5 + 0.5) : 0.25),
        position().y + (offset.y ? (1 - buttonHeight) * (offset.y * 0.5 + 0.5) : 0.25),
        position().z + (offset.z ? (1 - buttonHeight) * (offset.z * 0.5 + 0.5) : 0.25),
        offset.x ? buttonHeight : buttonWidth,
        offset.y ? buttonHeight : buttonWidth,
        offset.z ? buttonHeight : buttonWidth,
        QColor(192, 192, 192)
    );

    QGraphicsItem * buttonFace;
    switch (attachment) {
        case dirEast:
        case dirWest:
            buttonFace = ret.at(1);
            break;

        case dirNorth:
        case dirSouth:
            buttonFace = ret.at(2);
            break;

        default:
            buttonFace = ret.at(0);
            break;
    }

    //buttonFace->setAcceptedMouseButtons(Qt::LeftButton);
    QVariant v;
    v.setValue(static_cast<QObject*>(this));
    buttonFace->setData(123, v);

    return ret;
}

void BlockButton::clicked()
{
    on = !on;
    updateGeometry();
}
