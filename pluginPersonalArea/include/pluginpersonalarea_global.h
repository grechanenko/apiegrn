#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PLUGINPERSONALAREA_LIB)
#  define PLUGINPERSONALAREA_EXPORT Q_DECL_EXPORT
# else
#  define PLUGINPERSONALAREA_EXPORT Q_DECL_IMPORT
# endif
#else
# define PLUGINPERSONALAREA_EXPORT
#endif
