#ifndef WORLD_H
#define WORLD_H

#include "position.h"
#include "projector.h"
#include <QObject>
#include <QMap>
#include <QGraphicsScene>
#include <qnumeric.h>

class Block;

class World : public QObject
{
    Q_OBJECT
private:
    QMap<Position, Block *> blocks;

public:
    explicit World(QGraphicsScene * parent = 0);
    void insertBlock(Block * block);
    Position getCoords(Position const & position, double zOrderOverride = qInf());
    Projector projector;

signals:

private slots:
    void projectionChanged();
public slots:
    void updateGeometry();
};

#endif // WORLD_H
