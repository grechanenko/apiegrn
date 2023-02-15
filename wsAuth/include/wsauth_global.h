#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(WSAUTH_LIB)
#  define WSAUTH_EXPORT Q_DECL_EXPORT
# else
#  define WSAUTH_EXPORT Q_DECL_IMPORT
# endif
#else
# define WSAUTH_EXPORT
#endif
