#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PLUGINPAGECADASTER_LIB)
#  define PLUGINPAGECADASTER_EXPORT Q_DECL_EXPORT
# else
#  define PLUGINPAGECADASTER_EXPORT Q_DECL_IMPORT
# endif
#else
# define PLUGINPAGECADASTER_EXPORT
#endif
