#ifndef POSITION_H
#define POSITION_H

#include <QtGlobal>

class Position
{
public:
    qreal x, y, z;

public:
    Position();
    Position(qreal x, qreal y, qreal z);

    bool operator<(Position const & other) const {
        if (y == other.y) {
            if (x == other.x) {
                return z < other.z;
            }
            else {
                return x < other.x;
            }
        }
        else {
            return y < other.y;
        }

    }
};

#endif // POSITION_H
