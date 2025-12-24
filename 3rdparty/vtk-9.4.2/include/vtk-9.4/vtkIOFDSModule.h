
#ifndef VTKIOFDS_EXPORT_H
#define VTKIOFDS_EXPORT_H

#ifdef VTKIOFDS_STATIC_DEFINE
#  define VTKIOFDS_EXPORT
#  define VTKIOFDS_NO_EXPORT
#else
#  ifndef VTKIOFDS_EXPORT
#    ifdef IOFDS_EXPORTS
        /* We are building this library */
#      define VTKIOFDS_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define VTKIOFDS_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef VTKIOFDS_NO_EXPORT
#    define VTKIOFDS_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef VTKIOFDS_DEPRECATED
#  define VTKIOFDS_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef VTKIOFDS_DEPRECATED_EXPORT
#  define VTKIOFDS_DEPRECATED_EXPORT VTKIOFDS_EXPORT VTKIOFDS_DEPRECATED
#endif

#ifndef VTKIOFDS_DEPRECATED_NO_EXPORT
#  define VTKIOFDS_DEPRECATED_NO_EXPORT VTKIOFDS_NO_EXPORT VTKIOFDS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKIOFDS_NO_DEPRECATED
#    define VTKIOFDS_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkIOFDSModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKIOFDS_EXPORT_H */
