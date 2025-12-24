cmake_minimum_required(VERSION 3.16)
project(OpenCAX VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ------------------------------
# Qt6
# ------------------------------
find_package(Qt6 COMPONENTS Widgets REQUIRED)
if(NOT Qt6_FOUND)
    message(FATAL_ERROR "Qt6 not found")
else()
    message(STATUS "Using Qt6 Widgets from ${Qt6Widgets_DIR}")
endif()

# ------------------------------
# OpenCASCADE
# ------------------------------
set(OpenCASCADE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/occt-7.9.2/lib/cmake/opencascade")
find_package(OpenCASCADE REQUIRED)
if(NOT OpenCASCADE_FOUND)
    message(FATAL_ERROR "Could not find OpenCASCADE")
else()
    message(STATUS "Using OpenCASCADE from ${OpenCASCADE_INSTALL_PREFIX}")
endif()

include_directories(
    3rdparty/occt-7.9.2/include
    3rdparty/occt-7.9.2/include/opencascade
)
link_directories(3rdparty/occt-7.9.2/lib)

# ------------------------------
# VTK
# ------------------------------
set(VTK_DIR "${CMAKE_SOURCE_DIR}/3rdparty/vtk-9.4.2/lib/cmake/vtk-9.4")
find_package(VTK REQUIRED)
if(NOT VTK_FOUND)
    message(FATAL_ERROR "Could not find VTK")
endif()
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/vtk-9.4.2/include/vtk-9.3)
link_directories(${CMAKE_SOURCE_DIR}/3rdparty/vtk-9.4.2/lib)

# ------------------------------
# FreeImage (系统库)
# ------------------------------
find_path(FREEIMAGE_INCLUDE_DIR FreeImage.h HINTS /usr/include /usr/include/freeimage /usr/local/include)
find_library(FREEIMAGE_LIBRARY NAMES FreeImage freeimage HINTS /usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib)

if(NOT FREEIMAGE_INCLUDE_DIR OR NOT FREEIMAGE_LIBRARY)
    message(FATAL_ERROR "Could not find system FreeImage library. Did you install libfreeimage-dev?")
else()
    message(STATUS "Found FreeImage:")
    message(STATUS "  include: ${FREEIMAGE_INCLUDE_DIR}")
    message(STATUS "  library: ${FREEIMAGE_LIBRARY}")
endif()
include_directories(${FREEIMAGE_INCLUDE_DIR})

# ------------------------------
# FreeType (系统库)
# ------------------------------
find_path(FREETYPE_INCLUDE_DIR ft2build.h HINTS /usr/include/freetype2 /usr/local/include/freetype2)
find_library(FREETYPE_LIBRARY NAMES freetype freetype2 HINTS /usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib)

if(NOT FREETYPE_INCLUDE_DIR OR NOT FREETYPE_LIBRARY)
    message(FATAL_ERROR "Could not find system FreeType library. Did you install libfreetype6-dev?")
else()
    message(STATUS "Found FreeType:")
    message(STATUS "  include: ${FREETYPE_INCLUDE_DIR}")
    message(STATUS "  library: ${FREETYPE_LIBRARY}")
endif()
include_directories(${FREETYPE_INCLUDE_DIR})

# ------------------------------
# Tcl/Tk (系统库)
# ------------------------------
set(TCL_INCLUDE_PATH /usr/include/tcl8.6)
set(TCL_LIBRARY /usr/lib/x86_64-linux-gnu/libtcl8.6.so)

set(TK_INCLUDE_PATH /usr/include/tk8.6)
set(TK_LIBRARY /usr/lib/x86_64-linux-gnu/libtk8.6.so)

message(STATUS "Using system Tcl/Tk:")
message(STATUS "  Tcl include: ${TCL_INCLUDE_PATH}")
message(STATUS "  Tcl library: ${TCL_LIBRARY}")
message(STATUS "  Tk include: ${TK_INCLUDE_PATH}")
message(STATUS "  Tk library: ${TK_LIBRARY}")

include_directories(${TCL_INCLUDE_PATH} ${TK_INCLUDE_PATH})
#------------------------------
# SARibbon (第三方库)
# ------------------------------
set(SARIBBON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/SARibbon/include/SARibbonBar")
set(SARIBBON_LIBRARY "${CMAKE_SOURCE_DIR}/3rdparty/SARibbon/lib/libSARibbonBar.so.2.5.5")

if(EXISTS ${SARIBBON_INCLUDE_DIR} AND EXISTS ${SARIBBON_LIBRARY})
    message(STATUS "Found SARibbon library:")
    message(STATUS "  include: ${SARIBBON_INCLUDE_DIR}")
    message(STATUS "  library: ${SARIBBON_LIBRARY}")
else()
    message(FATAL_ERROR "Could not find SARibbon library in 3rdparty/SARibbon")
endif()
include_directories(${SARIBBON_INCLUDE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/3rdparty/SARibbon/lib)
# ------------------------------
# Project Sources
# ------------------------------
set(PROJECT_SOURCES
    main.cpp
    MainWindow.h
    MainWindow.cpp
    MainWindow.ui
    MVTKWidget.h
    MVkWidget.cpp
)

# ------------------------------
# Build target
# ------------------------------
qt_add_executable(OpenCAX
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}

)

target_link_libraries(OpenCAX PRIVATE
    Qt6::Widgets
    ${OpenCASCADE_LIBRARIES}
    ${VTK_LIBRARIES}
    ${FREEIMAGE_LIBRARY}
    ${FREETYPE_LIBRARY}
    ${SARIBBON_LIBRARY}   # 链接 SARibbon 库
    # ${TCL_LIBRARY}
    # ${TK_LIBRARY}
)

# macOS bundle config
if(${Qt6_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.OpenCAX)
endif()

set_target_properties(OpenCAX PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install
include(GNUInstallDirs)
install(TARGETS OpenCAX
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize for Qt6
qt_finalize_executable(OpenCAX)

# ------------------------------
# Test libraries found
# ------------------------------
# add_custom_target(test_libs ALL
#     COMMAND ${CMAKE_COMMAND} -E echo "Library paths check:"
#     COMMAND ${CMAKE_COMMAND} -E echo "  FreeImage: ${FREEIMAGE_LIBRARY}"
#     COMMAND ${CMAKE_COMMAND} -E echo "  FreeType: ${FREETYPE_LIBRARY}"
#     COMMAND ${CMAKE_COMMAND} -E echo "  Tcl: ${TCL_LIBRARY}"
#     COMMAND ${CMAKE_COMMAND} -E echo "  Tk: ${TK_LIBRARY}"
# )

