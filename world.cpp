#include "world.h"
#include "block.h"
#include "blockstone.h"
#include "blocktorch.h"
#include "blockbutton.h"
#include "blockwire.h"

#include <QFile>
#include <QTextStream>

World::World(QGraphicsScene * parent) :
    QObject(parent)
{
    connect(&projector, SIGNAL(changed()), SLOT(projectionChanged())); // TODO: move to helper function
}

World::World(QString const & fileName, QGraphicsScene * parent) :
    QObject(parent)
{
    connect(&projector, SIGNAL(changed()), SLOT(projectionChanged())); // TODO: move to helper function
    loadWorld(fileName);
}

void World::loadWorld(QString const & fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    QTextStream stream(&file);
    QString line;

    line = stream.readLine();

    QStringList arr = line.split('/');
    Q_ASSERT(arr.size() == 3);

    int worldXSize = arr.at(0).toInt();
    int worldYSize = arr.at(1).toInt();
    int worldZSize = arr.at(2).toInt();

    for (int yp = 0; yp < worldYSize; ++yp) {
        line = stream.readLine();
        Q_ASSERT(line.isEmpty()); // Empty line expected.
        for (int xp = 0; xp < worldXSize; ++xp) {
            line = stream.readLine();
            Q_ASSERT(line.size() == worldZSize * 2); // Unexpected line length

            for (int zp = 0; zp < worldZSize; ++zp) {
                vec3 position(xp-worldXSize/2, yp-worldYSize/2, zp-worldZSize/2);
                int linePos = (worldZSize - 1 - zp) * 2;

                Block * lastBlock = 0;
                switch (line[linePos].toLatin1()) {
                    case 'x':
                        lastBlock = new BlockStone(position, this);
                        break;

                    case 't':
                        lastBlock = new BlockTorch(position, this);
                        break;

                    case 'b':
                        lastBlock = new BlockButton(position, this);
                        break;

                    case 'r':
                        lastBlock = new BlockWire(position, this);
                        break;
                }

                if (!lastBlock) continue;

                switch (line[linePos+1].toLatin1()) {
                    case 'n':
                        lastBlock->attachment = dirNorth;
                        break;

                    case 's':
                        lastBlock->attachment = dirSouth;
                        break;

                    case 'e':
                        lastBlock->attachment = dirEast;
                        break;

                    case 'w':
                        lastBlock->attachment = dirWest;
                        break;

                    case 'd':
                        lastBlock->attachment = dirDown;
                        break;

                    case 'u':
                        lastBlock->attachment = dirUp;
                        break;
                }
            }
        }
    }
    updateGeometry();
}

void World::insertBlock(Block * block)
{
    blocks[block->position()] = block;
}

vec3 World::getCoords(vec3 const & position, double zOrderOverride) {
    vec3 ret = projector.project(position);
    if (!qIsInf(zOrderOverride)) ret.z = zOrderOverride;
    return ret;
}

void World::projectionChanged() {
    redraw();
}

void World::redraw() {
    foreach(Block * block, blocks) {
        block->redraw();
    }
}

void World::updateGeometry() {
    foreach(Block * block, blocks) {
        block->updateGeometry();
    }
}

bool World::sideVisible(Direction direction)
{
    vec3 normal = dirToOffset(direction);
    return projector.faceVisible(normal);
}

Block * World::blockAt(vec3 const & position)
{
    if (!blocks.contains(position)) return 0;

    return blocks[position];
}

void World::setTicked(Block * block, bool ticked)
{
    if (ticked) {
        tickedBlocks += block;
    }
    else {
        tickedBlocks -= block;
    }
}

void World::tick()
{
    foreach (Block * block, tickedBlocks) {
        block->tick();
    }
}
