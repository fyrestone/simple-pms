#ifndef CONTEXT_H
#define CONTEXT_H

#include "permission.h"
#include <QSettings>

class Context
{
public:
    Context();
    static Context *instance();

    Permission &curAccountPermission() {return permission;}
    QSettings &curAppSettings() {return settings;}

private:
    Permission permission;
    QSettings settings;
};

#endif // CONTEXT_H
