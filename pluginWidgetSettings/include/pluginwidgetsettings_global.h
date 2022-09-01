#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PLUGINWIDGETSETTINGS_LIB)
#  define PLUGINWIDGETSETTINGS_EXPORT Q_DECL_EXPORT
# else
#  define PLUGINWIDGETSETTINGS_EXPORT Q_DECL_IMPORT
# endif
#else
# define PLUGINWIDGETSETTINGS_EXPORT
#endif
