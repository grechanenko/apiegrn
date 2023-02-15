#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PLUGINUPDATER_LIB)
#  define PLUGINUPDATER_EXPORT Q_DECL_EXPORT
# else
#  define PLUGINUPDATER_EXPORT Q_DECL_IMPORT
# endif
#else
# define PLUGINUPDATER_EXPORT
#endif
