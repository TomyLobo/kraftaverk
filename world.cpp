#include "world.h"
#include "block.h"
#include "blockstone.h"
#include "blocktorch.h"
#include "blockbutton.h"
#include "blockwire.h"
#include "blockdoor.h"
#include "blockpressureplate.h"

#include "mathlib/vector4.h"
#include "mathlib/matrix4.h"

#include <QFile>
#include <QTextStream>
#include <QTimer>

World::World(QGraphicsScene * parent) :
    QObject(parent)
{
    init();
}

World::World(QString const & fileName, QObject * parent) :
    QObject(parent)
{
    init();
    loadWorld(fileName);
}
World::~World()
{
    // Make sure blocks are destroyed before there is no world anymore.
    QObjectList children(this->children());
    foreach(QObject * child, children) {
        delete child;
    }
}

void World::init()
{
    mDirty = true;
    QTimer * foo = new QTimer(this);
    connect(foo, SIGNAL(timeout()), SLOT(tick()));
    foo->setInterval(100);
    foo->start();
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

                    case 'd':
                        lastBlock = new BlockDoor(position, this);
                        break;

                    case 'p':
                        lastBlock = new BlockPressurePlate(position, this);
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
}

void World::insertBlock(Block * block)
{
    blocks[block->position()] = block;
}

void World::removeBlock(Block * block)
{
    blocks.remove(block->position());
}

void World::draw()
{
    GLdouble projection[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    GLdouble modelView[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelView);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    QMultiMap<qreal, Block *> blockMap;
    foreach(Block * block, blocks) {
        vec3 pos = block->position();
        qreal x, y, z;
        gluProject(pos.x, pos.y, pos.z, modelView, projection, viewport, &x, &y, &z);
        blockMap.insert(-z, block);
    }

    foreach(Block * block, blockMap) {
        block->draw();
    }
    mDirty = false;
}

Block * World::blockAt(vec3 const & position)
{
    if (!blocks.contains(position))
        return 0;

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

void World::setDirty()
{
    if (mDirty) return;

    mDirty = true;
    QTimer::singleShot(0, this, SIGNAL(redrawNeeded()));
}

QPair<Block *, Direction> World::getClosestFace(vec3 const & position)
{
    QPair<Block *, Direction> ret(0, dirNone);

    vec3 floorobj(floor(position.x), floor(position.y), floor(position.z));
    Block *block = blockAt(floorobj);
    if (!block) return ret;

    ret.first = block;

    QPair<vec3, vec3> box = block->boundingBox();

    vec3 offset = position-(box.first + box.second*0.5);
    offset /= box.second;

    Direction dir = offsetToDir(offset);
    ret.second = dir;

    return ret;
}

Block * World::addBlock(vec3 const & position, Block::BlockType blockType)
{
    switch (blockType) {
    case Block::btAir:
        return 0;

    case Block::btStone:
        return new BlockStone(position, this);

    case Block::btTorch:
        return new BlockTorch(position, this);

    case Block::btButton:
        return new BlockButton(position, this);

    case Block::btWire:
        return new BlockWire(position, this);

    case Block::btDoor:
        return new BlockDoor(position, this);

    case Block::btPressurePlate:
        return new BlockPressurePlate(position, this);
    case Block::btLever:
    default:
        return 0;
    }
}
