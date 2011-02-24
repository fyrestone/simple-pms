#include "context.h"

#include <QGlobalStatic>        /* 使用Q_GLOBAL_STATIC来单例化 */
#include <QApplication>

Q_GLOBAL_STATIC(Context, context)

Context::Context()
    :settings(QApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat)
{
}

Context *Context::instance()
{
    return context();
}
