
#ifndef VTKFILTERSTEMPORAL_EXPORT_H
#define VTKFILTERSTEMPORAL_EXPORT_H

#ifdef VTKFILTERSTEMPORAL_STATIC_DEFINE
#  define VTKFILTERSTEMPORAL_EXPORT
#  define VTKFILTERSTEMPORAL_NO_EXPORT
#else
#  ifndef VTKFILTERSTEMPORAL_EXPORT
#    ifdef FiltersTemporal_EXPORTS
        /* We are building this library */
#      define VTKFILTERSTEMPORAL_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define VTKFILTERSTEMPORAL_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef VTKFILTERSTEMPORAL_NO_EXPORT
#    define VTKFILTERSTEMPORAL_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef VTKFILTERSTEMPORAL_DEPRECATED
#  define VTKFILTERSTEMPORAL_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef VTKFILTERSTEMPORAL_DEPRECATED_EXPORT
#  define VTKFILTERSTEMPORAL_DEPRECATED_EXPORT VTKFILTERSTEMPORAL_EXPORT VTKFILTERSTEMPORAL_DEPRECATED
#endif

#ifndef VTKFILTERSTEMPORAL_DEPRECATED_NO_EXPORT
#  define VTKFILTERSTEMPORAL_DEPRECATED_NO_EXPORT VTKFILTERSTEMPORAL_NO_EXPORT VTKFILTERSTEMPORAL_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKFILTERSTEMPORAL_NO_DEPRECATED
#    define VTKFILTERSTEMPORAL_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkFiltersTemporalModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKFILTERSTEMPORAL_EXPORT_H */
