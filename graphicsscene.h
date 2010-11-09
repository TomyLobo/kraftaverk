#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = 0);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);

signals:

public slots:

};

#endif // GRAPHICSSCENE_H
