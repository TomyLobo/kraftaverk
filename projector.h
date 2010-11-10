#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "position.h"
#include "mathlib/matrix3.h"

#include <QObject>

class Projector : public QObject
{
    Q_OBJECT
private:
    matrix3<qreal> projection;
    matrix3<qreal> modelView;
    matrix3<qreal> combined;

    void combineMatrix();

public:
    explicit Projector(QObject *parent = 0);
    vec3 project(vec3 const & position);
    bool faceVisible(vec3 normal);

    void setYaw(qreal yaw);

signals:
    void changed();

public slots:

};

#endif // PROJECTOR_H
