#include "projector.h"

Projector::Projector(QObject *parent) :
    QObject(parent)
{
}


Position Projector::project(Position const & position)
{
    Position ret;
    ret.x = 2*position.x                - 2*position.z;
    ret.y =   position.x - 2*position.y +   position.z;
    ret.z =   position.x + 2*position.y +   position.z;

    ret.x *= 16;
    ret.y *= 16;

    return ret;
}

