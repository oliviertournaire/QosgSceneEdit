#ifndef SEQTEXT_H
#define SEQTEXT_H

#include <QtCore/qglobal.h>

#ifdef Q_OS_WIN
#   ifdef SEQTEXT_EXPORT_LIBRARY
#       define SEQTEXT_EXPORT __declspec(dllexport)
#   else
#       define SEQTEXT_EXPORT __declspec(dllimport)
#   endif
#else
#   define SEQTEXT_EXPORT
#endif

#endif // SEQTEXT_H
