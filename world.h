#ifndef WORLD_H
#define WORLD_H

#include "position.h"
#include "projector.h"
#include "helper.h"

#include <QObject>
#include <QMap>
#include <QGraphicsScene>
#include <qnumeric.h>

class Block;

class World : public QObject
{
    Q_OBJECT
private:
    QMap<vec3, Block *> blocks;

    void loadWorld(QString const & fileName);

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

signals:

private slots:
    void projectionChanged();
public slots:
    void updateGeometry();
};

#endif // WORLD_H
