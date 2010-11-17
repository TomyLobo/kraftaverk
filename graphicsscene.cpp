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
    if (mouseEvent->button() == Qt::RightButton) {

        mouseEvent->accept();
        return;
    }

    do {
        QGraphicsItem *item = itemAt(mouseEvent->scenePos());
        if (!item) break;

        if (!item->isEnabled()) break;
        if (!(item->acceptedMouseButtons() & mouseEvent->button())) break;

        Block * block = static_cast<Block*>(item->data(123).value<QObject*>());
        if (!block) break;

        block->clicked();

        mouseEvent->accept();
        return;
    } while (0);

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if (mouseEvent->buttons() & Qt::RightButton) {
        QPointF delta = mouseEvent->screenPos() - mouseEvent->lastScreenPos();

        emit mouseDragged(delta);

        mouseEvent->accept();
        return;
    }

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if (mouseEvent->button() == Qt::RightButton) {


        mouseEvent->accept();
        return;
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
