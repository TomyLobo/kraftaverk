#include "displaylist.h"

DisplayList::DisplayList() :
    name(0)
{
}

void DisplayList::initList()
{
    context = QGLContext::currentContext();
    name = glGenLists(1);
    Q_ASSERT(name != 0);
    Q_ASSERT(glGetError() != GL_INVALID_VALUE);
    Q_ASSERT(glGetError() != GL_INVALID_OPERATION);
}

DisplayList::~DisplayList()
{
    if (!name) return;
    Q_ASSERT(context == QGLContext::currentContext());
    glDeleteLists(name, 1);
    Q_ASSERT(glGetError() != GL_INVALID_VALUE);
    Q_ASSERT(glGetError() != GL_INVALID_OPERATION);
}

void DisplayList::call()
{
    if (!name) return;

    Q_ASSERT(context == QGLContext::currentContext());
    glCallList(name);
}

void DisplayList::begin()
{
    if (!name) initList();

    Q_ASSERT(context == QGLContext::currentContext());
    glNewList(name, GL_COMPILE);
    Q_ASSERT(glGetError() != GL_INVALID_VALUE);
    Q_ASSERT(glGetError() != GL_INVALID_ENUM);
    Q_ASSERT(glGetError() != GL_INVALID_OPERATION);
    Q_ASSERT(glGetError() != GL_OUT_OF_MEMORY);
}

void DisplayList::end()
{
    if (!name) return;
    Q_ASSERT(context == QGLContext::currentContext());
    glEndList();
    Q_ASSERT(glGetError() != GL_INVALID_OPERATION);
}
