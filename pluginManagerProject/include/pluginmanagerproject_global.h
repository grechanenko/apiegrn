#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PLUGINMANAGERPROJECT_LIB)
#  define PLUGINMANAGERPROJECT_EXPORT Q_DECL_EXPORT
# else
#  define PLUGINMANAGERPROJECT_EXPORT Q_DECL_IMPORT
# endif
#else
# define PLUGINMANAGERPROJECT_EXPORT
#endif
