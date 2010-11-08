#ifndef HELPER_H
#define HELPER_H

enum Direction {
    dirNone = 0,
    dirDown = 1,
    dirUp = 2,
    dirEast = 3,
    dirNorth = 4,
    dirWest = 5,
    dirSouth = 6
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

static inline Position dirToOffset(Direction direction)
{
    switch(direction) {
        case dirNone:  return Position( 0,  0,  0);
        case dirDown:  return Position( 0, -1,  0);
        case dirUp:    return Position( 0,  1,  0);
        case dirEast:  return Position( 0,  0, -1);
        case dirNorth: return Position(-1,  0,  0);
        case dirWest:  return Position( 0,  0,  1);
        case dirSouth: return Position( 1,  0,  0);
        default:
            Q_ASSERT(false);
    }
    return Position();
}

#endif // HELPER_H
