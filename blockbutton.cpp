#include "blockbutton.h"

BlockButton::BlockButton(Position const & position, World *parent) :
    Block(position, parent),
    on(false)
{
}

QList<QGraphicsItem *> BlockButton::getGeometry()
{
    QList<QGraphicsItem *> ret;

    qreal buttonHeight = on ? 0.1 : 0.25;
    qreal buttonWidth = 0.5;

    Position offset = dirToOffset(attachment);

    ret << boxhelper(
        position().x + (offset.x ? (1 - buttonHeight) * (offset.x * 0.5 + 0.5) : 0.25),
        position().y + (offset.y ? (1 - buttonHeight) * (offset.y * 0.5 + 0.5) : 0.25),
        position().z + (offset.z ? (1 - buttonHeight) * (offset.z * 0.5 + 0.5) : 0.25),
        offset.x ? buttonHeight : buttonWidth,
        offset.y ? buttonHeight : buttonWidth,
        offset.z ? buttonHeight : buttonWidth,
        QColor(192,192,192)
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

    /*
    With buttonFace
        .OnAction = "worldButton_Clicked"
        .Name = "x" & x & "x" & y & "x" & z & "_" & .Name
    End With
    */
    return ret;
}
