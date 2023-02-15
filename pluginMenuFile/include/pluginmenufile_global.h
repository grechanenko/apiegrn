#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PLUGINMENUFILE_LIB)
#  define PLUGINMENUFILE_EXPORT Q_DECL_EXPORT
# else
#  define PLUGINMENUFILE_EXPORT Q_DECL_IMPORT
# endif
#else
# define PLUGINMENUFILE_EXPORT
#endif
