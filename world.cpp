#include "world.h"
#include "block.h"

World::World(QGraphicsScene * parent) :
    QObject(parent)
{
    connect(&projector, SIGNAL(changed()), SLOT(projectionChanged()));
}

void World::insertBlock(Block * block)
{
    blocks[block->position()] = block;
}

Position World::getCoords(Position const & position, double zOrderOverride) {
    Position ret = projector.project(position);
    if (!qIsInf(zOrderOverride)) ret.z = zOrderOverride;
    return ret;
}

void World::projectionChanged() {
    updateGeometry();
}

void World::updateGeometry() {
    foreach(Block * block, blocks) {
        block->updateGeometry();
    }
}
