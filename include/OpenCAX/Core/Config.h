#pragma once

// =========================================
// Platform
// =========================================

#if defined(_WIN32) || defined(_WIN64)
    #define OpenCAX_PLATFORM_WINDOWS 1
    #define OpenCAX_PLATFORM_LINUX   0
#else
    #define OpenCAX_PLATFORM_WINDOWS 0
    #define OpenCAX_PLATFORM_LINUX   1
#endif

// =========================================
// Build type
// =========================================

#ifdef NDEBUG
    #define OpenCAX_RELEASE 1
    #define OpenCAX_DEBUG   0
#else
    #define OpenCAX_RELEASE 0
    #define OpenCAX_DEBUG   1
#endif

// =========================================
// Version
// =========================================

#define OpenCAX_VERSION_MAJOR 0
#define OpenCAX_VERSION_MINOR 1
#define OpenCAX_VERSION_PATCH 0
#define OpenCAX_VERSION_STRING "0.1.0"

// =========================================
// Logging
// =========================================

#ifndef OpenCAX_ENABLE_LOG
    #define OpenCAX_ENABLE_LOG 1
#endif

#ifndef OpenCAX_ENABLE_DEBUG_LOG
    #define OpenCAX_ENABLE_DEBUG_LOG OpenCAX_DEBUG
#endif

#ifndef OpenCAX_ENABLE_FILE_LOG
    #define OpenCAX_ENABLE_FILE_LOG 1
#endif

// =========================================
// Export
// =========================================

#if OpenCAX_PLATFORM_WINDOWS

    #ifdef OpenCAX_EXPORTS
        #define OpenCAX_API __declspec(dllexport)
    #else
        #define OpenCAX_API __declspec(dllimport)
    #endif

#else

    #define OpenCAX_API

#endif