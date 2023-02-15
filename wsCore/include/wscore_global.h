#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(WSCORE_LIB)
#  define WSCORE_EXPORT Q_DECL_EXPORT
# else
#  define WSCORE_EXPORT Q_DECL_IMPORT
# endif
#else
# define WSCORE_EXPORT
#endif
