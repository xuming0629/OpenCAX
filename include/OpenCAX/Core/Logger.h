#pragma once

#include <OpenCAX/Core/Config.h>

#include <memory>
#include <string>

namespace spdlog
{
class logger;
}

namespace OpenCAX
{

enum class LogLevel
{
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical,
    Off
};

enum class LogModule
{
    Core,
    Geometry,
    Topology,
    Meshing,
    IO,
    FEM,
    Solver,
    Visualization,
    Application,
    Unknown
};

class OpenCAX_API Logger
{
public:
    static void init(
        LogLevel level = LogLevel::Info,
        const std::string& log_file = "OpenCAX.log"
    );

    static void shutdown();

    static std::shared_ptr<spdlog::logger>& get();

    static void setLevel(LogLevel level);
    static void setLevel(const std::string& level);

    static const char* moduleName(LogModule module);

    static bool initialized();

private:
    static std::shared_ptr<spdlog::logger> s_logger;
    static bool s_initialized;
};

} // namespace OpenCAX