#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PLUGINFEEDBACK_LIB)
#  define PLUGINFEEDBACK_EXPORT Q_DECL_EXPORT
# else
#  define PLUGINFEEDBACK_EXPORT Q_DECL_IMPORT
# endif
#else
# define PLUGINFEEDBACK_EXPORT
#endif
