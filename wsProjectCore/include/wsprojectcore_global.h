#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(WSPROJECTCORE_LIB)
#  define WSPROJECTCORE_EXPORT Q_DECL_EXPORT
# else
#  define WSPROJECTCORE_EXPORT Q_DECL_IMPORT
# endif
#else
# define WSPROJECTCORE_EXPORT
#endif
