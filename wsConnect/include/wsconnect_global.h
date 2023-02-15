#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(WSCONNECT_LIB)
#  define WSCONNECT_EXPORT Q_DECL_EXPORT
# else
#  define WSCONNECT_EXPORT Q_DECL_IMPORT
# endif
#else
# define WSCONNECT_EXPORT
#endif
