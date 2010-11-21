#ifndef DISPLAYLIST_H
#define DISPLAYLIST_H

#include <QtOpenGL/QtOpenGL>

class DisplayList
{
private:
    GLuint name;

    void initList();

public:
    DisplayList();
    ~DisplayList();

    void call();
    void begin();
    void end();
};

#endif // DISPLAYLIST_H
