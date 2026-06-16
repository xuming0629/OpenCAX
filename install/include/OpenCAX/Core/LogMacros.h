#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Core/Logger.h>

#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#if OpenCAX_ENABLE_LOG

#define OpenCAX_LOG_TRACE(module, ...)                                      \
    ::OpenCAX::Logger::get()->trace("[{}] {}",                              \
        ::OpenCAX::Logger::moduleName(module), fmt::format(__VA_ARGS__))

#define OpenCAX_LOG_DEBUG(module, ...)                                      \
    do {                                                                    \
        if constexpr (OpenCAX_ENABLE_DEBUG_LOG) {                           \
            ::OpenCAX::Logger::get()->debug("[{}] {}",                      \
                ::OpenCAX::Logger::moduleName(module), fmt::format(__VA_ARGS__)); \
        }                                                                   \
    } while (0)

#define OpenCAX_LOG_INFO(module, ...)                                       \
    ::OpenCAX::Logger::get()->info("[{}] {}",                               \
        ::OpenCAX::Logger::moduleName(module), fmt::format(__VA_ARGS__))

#define OpenCAX_LOG_WARN(module, ...)                                       \
    ::OpenCAX::Logger::get()->warn("[{}] {}",                               \
        ::OpenCAX::Logger::moduleName(module), fmt::format(__VA_ARGS__))

#define OpenCAX_LOG_ERROR(module, ...)                                      \
    ::OpenCAX::Logger::get()->error("[{}] {}",                              \
        ::OpenCAX::Logger::moduleName(module), fmt::format(__VA_ARGS__))

#define OpenCAX_LOG_CRITICAL(module, ...)                                   \
    ::OpenCAX::Logger::get()->critical("[{}] {}",                           \
        ::OpenCAX::Logger::moduleName(module), fmt::format(__VA_ARGS__))

#else

#define OpenCAX_LOG_TRACE(module, ...)
#define OpenCAX_LOG_DEBUG(module, ...)
#define OpenCAX_LOG_INFO(module, ...)
#define OpenCAX_LOG_WARN(module, ...)
#define OpenCAX_LOG_ERROR(module, ...)
#define OpenCAX_LOG_CRITICAL(module, ...)

#endif

#define OpenCAX_CORE_TRACE(...) OpenCAX_LOG_TRACE(::OpenCAX::LogModule::Core, __VA_ARGS__)
#define OpenCAX_CORE_DEBUG(...) OpenCAX_LOG_DEBUG(::OpenCAX::LogModule::Core, __VA_ARGS__)
#define OpenCAX_CORE_INFO(...)  OpenCAX_LOG_INFO (::OpenCAX::LogModule::Core, __VA_ARGS__)
#define OpenCAX_CORE_WARN(...)  OpenCAX_LOG_WARN (::OpenCAX::LogModule::Core, __VA_ARGS__)
#define OpenCAX_CORE_ERROR(...) OpenCAX_LOG_ERROR(::OpenCAX::LogModule::Core, __VA_ARGS__)

#define OpenCAX_GEOMETRY_INFO(...) OpenCAX_LOG_INFO(::OpenCAX::LogModule::Geometry, __VA_ARGS__)
#define OpenCAX_MESH_INFO(...)     OpenCAX_LOG_INFO(::OpenCAX::LogModule::Mesh, __VA_ARGS__)
#define OpenCAX_IO_INFO(...)       OpenCAX_LOG_INFO(::OpenCAX::LogModule::IO, __VA_ARGS__)
#define OpenCAX_FEM_INFO(...)      OpenCAX_LOG_INFO(::OpenCAX::LogModule::FEM, __VA_ARGS__)
#define OpenCAX_SOLVER_INFO(...)   OpenCAX_LOG_INFO(::OpenCAX::LogModule::Solver, __VA_ARGS__)
#define OpenCAX_POST_INFO(...)     OpenCAX_LOG_INFO(::OpenCAX::LogModule::Post, __VA_ARGS__)

#define OpenCAX_GMSH_INFO(...)   OpenCAX_LOG_INFO(::OpenCAX::LogModule::MeshGmsh, __VA_ARGS__)
#define OpenCAX_TETGEN_INFO(...) OpenCAX_LOG_INFO(::OpenCAX::LogModule::MeshTetGen, __VA_ARGS__)
#define OpenCAX_VIEWER_INFO(...) OpenCAX_LOG_INFO(::OpenCAX::LogModule::PostViewer, __VA_ARGS__)