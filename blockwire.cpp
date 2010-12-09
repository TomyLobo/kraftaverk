#include "blockwire.h"
#include "world.h"

BlockWire::BlockWire(vec3 const & position, World *parent) :
    Block(position, parent),
    lastCharge(0)
{
}

Block::BlockType BlockWire::type()
{
    return btWire;
}

void BlockWire::drawGeometry()
{
    //ret << boxhelper(position().x, position().y, position().z, 1.0, .1, 1.0, QBrush(QColor(192, 0, 0)));
    QList<vec3> wireTargets;
    foreach (Block * wireTarget, neighboringWires(true)) {
        wireTargets << wireTarget->position();
    }

    switch (wireTargets.size()) {
    case 0:
        // 0 attachments: automatically add 4 implicit attachments
        for(int i = dirFirstFlat; i <= dirLastFlat; ++i) {
            vec3 offset = dirToOffset(static_cast<Direction>(i));
            wireTargets << position() + offset;
        }
        break;

    case 1:
        // 1 attachment: automatically add an implicit attachment to the opposite direction
        wireTargets << position()*vec3(2, 1, 2) + wireTargets.first() * vec3(-1, 0, -1);
    }
    QPen wirePen;
    wirePen.setWidth(3);
    wirePen.setColor(lastCharge ? QColor(255, 0, 0) : QColor(64, 32, 0));

    glhApplyPen(wirePen);

    glBegin(GL_LINES);
    foreach (vec3 wireTarget, wireTargets) {
        if (wireTarget.y > position().y) {
            // 2-step if y2 > y1
            glhVertex(position() + vec3(0.5, 0.1, 0.5));
            glhVertex(position() * vec3(0.55, 1, 0.55) + vec3(0.5, 0.1, 0.5) + wireTarget * vec3(0.45, 0, 0.45));
            glhVertex(position() * vec3(0.55, 1, 0.55) + vec3(0.5, 0.1, 0.5) + wireTarget * vec3(0.45, 0, 0.45));
            glhVertex(position() * vec3(0.55, 0, 0.55) + vec3(0.5, 0.1, 0.5) + wireTarget * vec3(0.45, 1, 0.45));
        }
        else {
            glhVertex(position() + vec3(0.5, 0.1, 0.5));
            glhVertex(position() * vec3(0.5, 1, 0.5) + vec3(0.5, 0.1, 0.5) + wireTarget * vec3(0.5, 0, 0.5));
        }
    }
    glEnd();

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    boxhelper(boundingBox(), QBrush(), false);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

QList<Block *> BlockWire::neighboringWires(bool devicesToo)
{
    QList<Block *> ret;

    if (!world()) return ret;

    Block * blockAbove = world()->blockAt(position() + vec3(0, 1, 0));
    bool aboveBlocked = blockAbove && blockAbove->allowsAttachment();

    for (int i = dirFirstFlat; i <= dirLastFlat; ++i) {
        vec3 offset = dirToOffset(static_cast<Direction>(i));
        Block * block = world()->blockAt(position() + offset);
        BlockType blockType = btAir;
        if (block) blockType = block->type();
        switch (blockType) {
        case btWire:
            ret << block;
            break;

        case btTorch:
        case btButton:
        case btPressurePlate:
        case btLever:
            if (devicesToo) {
                ret << block;
                break;
            }
            /* fall-through */
        default:
            if (block && block->allowsAttachment()) {
                // try block above it
                if (!aboveBlocked) {
                    Block * blockUp = world()->blockAt(position() + offset + vec3(0, 1, 0));
                    if (blockUp && blockUp->type() == btWire) {
                        ret << blockUp;
                    }
                }
            }
            else {
                // try block below it
                Block * blockDown = world()->blockAt(position() + offset + vec3(0, -1, 0));
                if (blockDown && blockDown->type() == btWire) {
                    ret << blockDown;
                }
            }
            break;
        }
    }
    return ret;
}

QList<BlockWire *> BlockWire::unchargeConnectedWires()
{
    QList<BlockWire *> sources;
    QSet<BlockWire *> cleared;
    QList<Block *> toClear = neighboringWires();

    while (!toClear.isEmpty()){
        Block * block = toClear.takeLast();
        Q_ASSERT(dynamic_cast<BlockWire *>(block));
        BlockWire * wire = static_cast<BlockWire *>(block);

        if (cleared.contains(wire)) continue;

        if (wire->lastCharge == 15) {
            sources << wire;
            continue; // might work too and be faster
        }
        else {
            wire->setCharge(0);
            wire->setDirty();
            cleared << wire;
        }
        toClear << wire->neighboringWires();
    }
    return sources;
}

bool BlockWire::validPowerSource(Block * poweredFrom, Block * poweredVia)
{
    // Wires can't be powered by wires. their charge is propagated elsewhere.
    return poweredFrom->type() != btWire;
}

void BlockWire::setPower(bool on)
{
    if (on) {
        setCharge(15);
    }
    else {
        setCharge(0);
        QList<BlockWire *> sources = unchargeConnectedWires();
        foreach (BlockWire * source, sources) {
            source->setCharge(15);
        }
    }
}

void BlockWire::setCharge(int charge)
{
    lastCharge = charge;
    if (charge > 0) --charge;

    QList<Block *> wireTargets = neighboringWires();

    foreach (Block * block, wireTargets) {
        Q_ASSERT(block->type() == btWire);
        BlockWire * wire = static_cast<BlockWire *>(block);

        if (wire->lastCharge < charge) {
            wire->setCharge(charge);
        }
    }

    QList<Block*> implicitWireTargets;

    switch (wireTargets.size()){
    case 0:
        // 0 attachments: automatically add 4 implicit attachments
        for(int i = dirFirstFlat; i <= dirLastFlat; ++i) {
            vec3 offset = dirToOffset(static_cast<Direction>(i));
            Block * block = world()->blockAt(position() + offset);
            if (block) implicitWireTargets << block;
        }
        break;

    case 1: {
            // 1 attachment: automatically add an implicit attachment to the opposite direction
            Block * block = world()->blockAt(position()*vec3(2, 1, 2) + wireTargets.first()->position() * vec3(-1, 0, -1));
            if (block) implicitWireTargets << block;
        }
    }
    implicitWireTargets << world()->blockAt(position() + vec3(0, -1, 0));

    foreach (Block * block, implicitWireTargets) {
        Q_ASSERT(block);
        block->setPower(lastCharge, this, 0);
        powerAllAround(block->position(), lastCharge, this, block);
    }

    setDirty();
}

BoundingBox BlockWire::boundingBox()
{
    return qMakePair(position(), vec3(1.0, 1.0, 1.0));
}

bool BlockWire::validatePlacement(const vec3 & where, Block *onWhat)
{
    Block * below = world()->blockAt(position()+vec3(0, -1, 0));
    return below && below->allowsAttachment();
}
