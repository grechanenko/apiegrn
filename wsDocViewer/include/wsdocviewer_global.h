#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(WSDOCVIEWER_LIB)
#  define WSDOCVIEWER_EXPORT Q_DECL_EXPORT
# else
#  define WSDOCVIEWER_EXPORT Q_DECL_IMPORT
# endif
#else
# define WSDOCVIEWER_EXPORT
#endif