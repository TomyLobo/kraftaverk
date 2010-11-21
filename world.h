#ifndef WORLD_H
#define WORLD_H

#include "position.h"
#include "helper.h"

#include <QObject>
#include <QMap>
#include <QGraphicsScene>
#include <qnumeric.h>
#include <QSet>

class Block;

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
    void insertBlock(Block * block);

    Block * blockAt(vec3 const & position);
    void setTicked(Block *, bool ticked);

    void setDirty();

signals:
    void redrawNeeded();

private slots:
public slots:
    void draw();
    void tick();
};

#endif // WORLD_H
