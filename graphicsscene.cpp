#include "graphicsscene.h"

#include "block.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
}


void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    /*QList<QGraphicsItem *> itemsUnderCursor = items(mouseEvent->scenePos());
    if (itemsUnderCursor.isEmpty()) return;

    QGraphicsItem *item = itemsUnderCursor.at(0);*/
    QGraphicsItem *item = itemAt(mouseEvent->scenePos());
    if (!item) return;

    if (!item->isEnabled()) return;
    if (!(item->acceptedMouseButtons() & mouseEvent->button())) return;

    Block * block = static_cast<Block*>(item->data(123).value<QObject*>());
    if (!block) return;

    block->clicked();

    qDebug("clicked!");

    QGraphicsScene::mousePressEvent(mouseEvent);
}
