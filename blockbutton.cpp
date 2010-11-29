#include "blockbutton.h"
#include "world.h"

BlockButton::BlockButton(vec3 const & position, World *parent) :
    Block(position, parent),
    mOn(false)
{
}

Block::BlockType BlockButton::type()
{
    return btButton;
}

BoundingBox BlockButton::boundingBox()
{
    qreal buttonHeight = on() ? 0.1 : 0.25;
    qreal buttonWidth = 0.5;

    vec3 offset = dirToOffset(attachment);

    return qMakePair(
        vec3(
            position().x + (offset.x ? (1 - buttonHeight) * (offset.x * 0.5 + 0.5) : 0.25),
            position().y + (offset.y ? (1 - buttonHeight) * (offset.y * 0.5 + 0.5) : 0.25),
            position().z + (offset.z ? (1 - buttonHeight) * (offset.z * 0.5 + 0.5) : 0.25)
        ),
        vec3(
            offset.x ? buttonHeight : buttonWidth,
            offset.y ? buttonHeight : buttonWidth,
            offset.z ? buttonHeight : buttonWidth
        )
    );
}

void BlockButton::drawGeometry()
{
    boxhelper(
        boundingBox(),
        QColor(192, 192, 192)
    );

    /*
    // TODO: adapt for clickables
    DShape * buttonFace;
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
    buttonFace->setData(123, v);*/
}

bool BlockButton::on()
{
    return mOn;
}

void BlockButton::setOn(bool on)
{
    if (mOn == on) return;

    mOn = on;

    vec3 attachedToPosition = position() + dirToOffset(static_cast<Direction>(attachment));

    Block * attachedTo = world()->blockAt(attachedToPosition);

    Q_ASSERT(attachedTo && attachedTo->allowsAttachment());

    powerAllAround(position(), mOn, this, 0);
    powerAllAround(attachedToPosition, mOn, this, attachedTo);
}

bool BlockButton::clicked()
{
    setOn(!on());

    setDirty();

    return true;
}
