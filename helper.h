#ifndef HELPER_H
#define HELPER_H

#include <QBrush>
#include "position.h"
#include <QtOpenGL/QtOpenGL>

enum Direction {
    dirNone = 0,
    dirDown = 1,
    dirUp = 2,
    dirEast = 3,
    dirNorth = 4,
    dirWest = 5,
    dirSouth = 6,
    dirFirstFlat = dirEast,
    dirLastFlat = dirSouth,
    dirFirstAll = dirDown,
    dirLastAll = dirSouth
};

enum DirectionAdd {
    daClockwise = -1,
    daCounterClockwise = 1
};

static inline Direction dirAdd(Direction direction, DirectionAdd directionAdd)
{
    int ret = direction + directionAdd;
    if (ret < dirEast) {
        ret += 4;
    }
    else if (ret > dirSouth) {
        ret -= 4;
    }

    return static_cast<Direction>(ret);
}

static inline vec3 dirToOffset(Direction direction)
{
    switch(direction) {
        case dirNone:  return vec3( 0,  0,  0);
        case dirDown:  return vec3( 0, -1,  0);
        case dirUp:    return vec3( 0,  1,  0);
        case dirEast:  return vec3( 0,  0, -1);
        case dirNorth: return vec3(-1,  0,  0);
        case dirWest:  return vec3( 0,  0,  1);
        case dirSouth: return vec3( 1,  0,  0);
        default:
            Q_ASSERT(false);
    }
    return vec3();
}

static inline void glhApplyBrush(QBrush const & brush)
{
    QColor color = brush.color();
    glColor4d(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

static inline void glhApplyPen(QPen const & pen)
{
    glhApplyBrush(pen.brush());
    glLineWidth(pen.widthF());
}

static inline void glhVertex(vec3 const & v)
{
    glVertex3d(v.x, v.y, v.z);
}

#endif // HELPER_H
