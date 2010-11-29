#ifndef WORLD_H
#define WORLD_H

#include "position.h"
#include "helper.h"
#include "block.h"

#include <QObject>
#include <QMap>
#include <QGraphicsScene>
#include <qnumeric.h>
#include <QSet>

class World : public QObject
{
    Q_OBJECT
private:
    QMap<vec3, Block *> blocks;

    void init();
    void loadWorld(QString const & fileName);
    QSet<Block*> tickedBlocks;

    bool mDirty;

public:
    explicit World(QGraphicsScene * parent = 0);
    World(QString const & fileName, QObject * parent = 0);
    ~World();

    void insertBlock(Block * block);
    void removeBlock(Block * block);

    Block * addBlock(vec3 const & position, Block::BlockType);

    Block * blockAt(vec3 const & position);
    void setTicked(Block *, bool ticked);

    void setDirty();

    QPair<Block *, Direction> getClosestFace(vec3 const & obj);

signals:
    void redrawNeeded();

private slots:
public slots:
    void draw();
    void tick();
};

#endif // WORLD_H
