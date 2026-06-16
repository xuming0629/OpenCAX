#include <OpenCAX/Core/Logger.h>

#include <algorithm>
#include <cctype>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#if OpenCAX_ENABLE_FILE_LOG
#include <spdlog/sinks/basic_file_sink.h>
#endif

namespace OpenCAX
{

std::shared_ptr<spdlog::logger> Logger::s_logger = nullptr;
bool Logger::s_initialized = false;

static spdlog::level::level_enum toSpdlogLevel(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Trace:    return spdlog::level::trace;
    case LogLevel::Debug:    return spdlog::level::debug;
    case LogLevel::Info:     return spdlog::level::info;
    case LogLevel::Warn:     return spdlog::level::warn;
    case LogLevel::Error:    return spdlog::level::err;
    case LogLevel::Critical: return spdlog::level::critical;
    case LogLevel::Off:      return spdlog::level::off;
    default:                 return spdlog::level::info;
    }
}

static std::string toLower(std::string value)
{
    std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        }
    );

    return value;
}

void Logger::init(LogLevel level, const std::string& log_file)
{
    if (s_initialized && s_logger)
    {
        return;
    }

    std::vector<spdlog::sink_ptr> sinks;

    auto console_sink =
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    console_sink->set_pattern(
        "%^[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v%$"
    );

    sinks.push_back(console_sink);

#if OpenCAX_ENABLE_FILE_LOG
    if (!log_file.empty())
    {
        auto file_sink =
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                log_file,
                true
            );

        file_sink->set_pattern(
            "[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v"
        );

        sinks.push_back(file_sink);
    }
#endif

    s_logger = std::make_shared<spdlog::logger>(
        "OpenCAX",
        sinks.begin(),
        sinks.end()
    );

    s_logger->set_level(toSpdlogLevel(level));
    s_logger->flush_on(spdlog::level::warn);

    spdlog::register_logger(s_logger);
    spdlog::set_default_logger(s_logger);

    s_initialized = true;
}

void Logger::shutdown()
{
    if (s_logger)
    {
        s_logger->flush();
    }

    spdlog::shutdown();

    s_logger.reset();
    s_initialized = false;
}

std::shared_ptr<spdlog::logger>& Logger::get()
{
    if (!s_initialized || !s_logger)
    {
#if OpenCAX_DEBUG
        init(LogLevel::Debug);
#else
        init(LogLevel::Info);
#endif
    }

    return s_logger;
}

void Logger::setLevel(LogLevel level)
{
    get()->set_level(toSpdlogLevel(level));
}

void Logger::setLevel(const std::string& level)
{
    const std::string value = toLower(level);

    if (value == "trace")
    {
        setLevel(LogLevel::Trace);
    }
    else if (value == "debug")
    {
        setLevel(LogLevel::Debug);
    }
    else if (value == "info")
    {
        setLevel(LogLevel::Info);
    }
    else if (value == "warn" || value == "warning")
    {
        setLevel(LogLevel::Warn);
    }
    else if (value == "error" || value == "err")
    {
        setLevel(LogLevel::Error);
    }
    else if (value == "critical")
    {
        setLevel(LogLevel::Critical);
    }
    else if (value == "off")
    {
        setLevel(LogLevel::Off);
    }
}

const char* Logger::moduleName(LogModule module)
{
    switch (module)
    {
    case LogModule::Core:
        return "Core";

    case LogModule::Geometry:
        return "Geometry";
    case LogModule::Topology:
        return "Geometry.Topology";
    case LogModule::Curve:
        return "Geometry.Curve";
    case LogModule::Surface:
        return "Geometry.Surface";
    case LogModule::GeometryOperation:
        return "Geometry.Operation";

    case LogModule::Mesh:
        return "Mesh";
    case LogModule::MeshGmsh:
        return "Mesh.Gmsh";
    case LogModule::MeshTetGen:
        return "Mesh.TetGen";
    case LogModule::MeshNetgen:
        return "Mesh.Netgen";
    case LogModule::MeshQuality:
        return "Mesh.Quality";
    case LogModule::MeshField:
        return "Mesh.Field";

    case LogModule::IO:
        return "IO";
    case LogModule::StepIO:
        return "IO.STEP";
    case LogModule::IgesIO:
        return "IO.IGES";
    case LogModule::BrepIO:
        return "IO.BREP";
    case LogModule::StlIO:
        return "IO.STL";
    case LogModule::VtkIO:
        return "IO.VTK";

    case LogModule::FEM:
        return "FEM";
    case LogModule::Material:
        return "FEM.Material";
    case LogModule::BoundaryCondition:
        return "FEM.BoundaryCondition";
    case LogModule::Load:
        return "FEM.Load";
    case LogModule::Assembly:
        return "FEM.Assembly";

    case LogModule::Solver:
        return "Solver";
    case LogModule::LinearSolver:
        return "Solver.Linear";
    case LogModule::NonlinearSolver:
        return "Solver.Nonlinear";
    case LogModule::ModalSolver:
        return "Solver.Modal";

    case LogModule::Post:
        return "Post";
    case LogModule::PostViewer:
        return "Post.Viewer";
    case LogModule::PostFilter:
        return "Post.Filter";
    case LogModule::PostActor:
        return "Post.Actor";
    case LogModule::PostExport:
        return "Post.Export";

    case LogModule::Application:
        return "Application";

    default:
        return "Unknown";
    }
}

bool Logger::initialized()
{
    return s_initialized;
}

} // namespace OpenCAX