#ifndef WORLD_H
#define WORLD_H

#include "position.h"
#include "projector.h"
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

public:
    QGraphicsScene * scene()
    {
        return static_cast<QGraphicsScene*>(parent());
    }
    explicit World(QGraphicsScene * parent = 0);
    World(QString const & fileName, QGraphicsScene * parent = 0);
    void insertBlock(Block * block);
    vec3 getCoords(vec3 const & position, double zOrderOverride = qInf());
    bool sideVisible(Direction direction);
    Projector projector;

    Block * blockAt(vec3 const & position);
    void setTicked(Block *, bool ticked);

signals:

private slots:
    void projectionChanged();
public slots:
    void redraw();
    void updateGeometry();
    void tick();
};

#endif // WORLD_H
