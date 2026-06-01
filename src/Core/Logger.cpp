#include <OpenCAX/Core/Logger.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#if OpenCAX_ENABLE_FILE_LOG
#include <spdlog/sinks/basic_file_sink.h>
#endif

#include <vector>

namespace OpenCAX
{

std::shared_ptr<spdlog::logger> Logger::s_logger = nullptr;
bool Logger::s_initialized = false;

static spdlog::level::level_enum toSpdlogLevel(LogLevel level)
{
    switch (level) {
    case LogLevel::Trace:
        return spdlog::level::trace;
    case LogLevel::Debug:
        return spdlog::level::debug;
    case LogLevel::Info:
        return spdlog::level::info;
    case LogLevel::Warn:
        return spdlog::level::warn;
    case LogLevel::Error:
        return spdlog::level::err;
    case LogLevel::Critical:
        return spdlog::level::critical;
    case LogLevel::Off:
        return spdlog::level::off;
    default:
        return spdlog::level::info;
    }
}

// void Logger::init(LogLevel level, const std::string& log_file)
// {
//     if (s_initialized && s_logger) {
//         return;
//     }

//     std::vector<spdlog::sink_ptr> sinks;

//     auto console_sink =
//         std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

//     console_sink->set_pattern(
//     "%^[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v%$"
// );

//     sinks.push_back(console_sink);

// #if OpenCAX_ENABLE_FILE_LOG
//     auto file_sink =
//         std::make_shared<spdlog::sinks::basic_file_sink_mt>(
//             log_file,
//             true
//         );

//     file_sink->set_pattern(
//     "[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v"
// );

//     sinks.push_back(file_sink);
// #endif

//     s_logger = std::make_shared<spdlog::logger>(
//         "OpenCAX",
//         sinks.begin(),
//         sinks.end()
//     );

//     s_logger->set_level(toSpdlogLevel(level));
//     s_logger->flush_on(spdlog::level::warn);

//     spdlog::register_logger(s_logger);
//     spdlog::set_default_logger(s_logger);

//     s_initialized = true;
// }

void Logger::init(LogLevel level, const std::string& log_file)
{
    if (s_initialized && s_logger) {
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
    if (!log_file.empty()) {
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
    if (s_logger) {
        s_logger->flush();
    }

    spdlog::shutdown();

    s_logger.reset();
    s_initialized = false;
}

std::shared_ptr<spdlog::logger>& Logger::get()
{
    if (!s_initialized || !s_logger) {
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
    if (level == "trace") {
        setLevel(LogLevel::Trace);
    } else if (level == "debug") {
        setLevel(LogLevel::Debug);
    } else if (level == "info") {
        setLevel(LogLevel::Info);
    } else if (level == "warn") {
        setLevel(LogLevel::Warn);
    } else if (level == "error") {
        setLevel(LogLevel::Error);
    } else if (level == "critical") {
        setLevel(LogLevel::Critical);
    } else if (level == "off") {
        setLevel(LogLevel::Off);
    }
}

const char* Logger::moduleName(LogModule module)
{
    switch (module) {
    case LogModule::Core:
        return "Core";
    case LogModule::Geometry:
        return "Geometry";
    case LogModule::Topology:
        return "Topology";
    case LogModule::Meshing:
        return "Meshing";
    case LogModule::IO:
        return "IO";
    case LogModule::FEM:
        return "FEM";
    case LogModule::Solver:
        return "Solver";
    case LogModule::Visualization:
        return "Visualization";
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