#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "position.h"

#include <QObject>

class Projector : public QObject
{
    Q_OBJECT
public:
    explicit Projector(QObject *parent = 0);
    Position project(Position const & position);

signals:
    void changed();

public slots:

};

#endif // PROJECTOR_H
