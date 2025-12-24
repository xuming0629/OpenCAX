
#ifndef VTKFILTERSCELLGRID_EXPORT_H
#define VTKFILTERSCELLGRID_EXPORT_H

#ifdef VTKFILTERSCELLGRID_STATIC_DEFINE
#  define VTKFILTERSCELLGRID_EXPORT
#  define VTKFILTERSCELLGRID_NO_EXPORT
#else
#  ifndef VTKFILTERSCELLGRID_EXPORT
#    ifdef FiltersCellGrid_EXPORTS
        /* We are building this library */
#      define VTKFILTERSCELLGRID_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define VTKFILTERSCELLGRID_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef VTKFILTERSCELLGRID_NO_EXPORT
#    define VTKFILTERSCELLGRID_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef VTKFILTERSCELLGRID_DEPRECATED
#  define VTKFILTERSCELLGRID_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef VTKFILTERSCELLGRID_DEPRECATED_EXPORT
#  define VTKFILTERSCELLGRID_DEPRECATED_EXPORT VTKFILTERSCELLGRID_EXPORT VTKFILTERSCELLGRID_DEPRECATED
#endif

#ifndef VTKFILTERSCELLGRID_DEPRECATED_NO_EXPORT
#  define VTKFILTERSCELLGRID_DEPRECATED_NO_EXPORT VTKFILTERSCELLGRID_NO_EXPORT VTKFILTERSCELLGRID_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKFILTERSCELLGRID_NO_DEPRECATED
#    define VTKFILTERSCELLGRID_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkFiltersCellGridModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKFILTERSCELLGRID_EXPORT_H */
