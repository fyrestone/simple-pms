#include "context.h"

#include <QGlobalStatic>        /* 使用Q_GLOBAL_STATIC来单例化 */

Q_GLOBAL_STATIC(Context, context)

Context::Context()
{
}

Context *Context::instance()
{
    return context();
}
