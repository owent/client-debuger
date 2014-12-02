#ifndef DEBUGER_GLOBAL_H
#define DEBUGER_GLOBAL_H

#include <string>
#include <cstring>
#include <memory>
#include <map>
#include <vector>
#include <QtCore/qglobal.h>

#ifdef DEBUGER_LIB
# define DEBUGER_EXPORT Q_DECL_EXPORT
#else
# define DEBUGER_EXPORT Q_DECL_IMPORT
#endif

#endif // DEBUGER_GLOBAL_H
