#ifndef CONTEXT_H
#define CONTEXT_H

#include "permission.h"

class Context
{
public:
    Context();
    static Context *instance();

private:
    Permission permission;
};

#endif // CONTEXT_H
