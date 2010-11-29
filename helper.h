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
static inline Direction offsetToDir(vec3 const & offset)
{
    if (offset.x == 0 && offset.y == 0 && offset.z == 0)
        return dirNone;

    if (fabs(offset.x) > fabs(offset.y)) {
        if (fabs(offset.z) > fabs(offset.x)) {
            // z highest
            return offset.z > 0 ? dirWest : dirEast;
        }
        else {
            // x highest
            return offset.x > 0 ? dirSouth : dirNorth;
        }
    }
    else {
        if (fabs(offset.z) > fabs(offset.y)) {
            // z highest
            return offset.z > 0 ? dirWest : dirEast;
        }
        else {
            // y highest (maybe x)
            return offset.y > 0 ? dirUp : dirDown;
        }
    }
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

#define DECOMPOSEV3(v) (v).x, (v).y, (v).z

static inline void glhVertex(vec3 const & v)
{
    glVertex3d(DECOMPOSEV3(v));
}

#endif // HELPER_H
