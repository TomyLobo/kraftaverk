#include "projector.h"

Projector::Projector(QObject *parent) :
    QObject(parent),
    projection(
        2,  0, -2,
        1, -2,  1,
        1,  2,  1
    ),
    modelView(
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    )
{
    projection *= 16;
    combineMatrix();
}

Position Projector::project(Position const & position)
{
    return combined * position;
}

void Projector::combineMatrix()
{
    combined = projection * modelView;
    emit changed();
}

void Projector::setYaw(qreal yaw)
{
    qreal cy = cos(yaw);
    qreal sy = sin(yaw);

    modelView = matrix3<qreal>(
        cy, 0, -sy,
         0, 1,   0,
        sy, 0,  cy
    );
    combineMatrix();
}
