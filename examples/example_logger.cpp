#include <OpenCAX/Core/Logger.h>
#include <OpenCAX/Core/LogMacros.h>

#include <iostream>
#include <string>

static OpenCAX::LogLevel parseLogLevel(const std::string& level)
{
    if (level == "trace") {
        return OpenCAX::LogLevel::Trace;
    }
    if (level == "debug") {
        return OpenCAX::LogLevel::Debug;
    }
    if (level == "info") {
        return OpenCAX::LogLevel::Info;
    }
    if (level == "warn") {
        return OpenCAX::LogLevel::Warn;
    }
    if (level == "error") {
        return OpenCAX::LogLevel::Error;
    }
    if (level == "critical") {
        return OpenCAX::LogLevel::Critical;
    }
    if (level == "off") {
        return OpenCAX::LogLevel::Off;
    }

    return OpenCAX::LogLevel::Info;
}

static void printHelp()
{
    std::cout
        << "OpenCAX logger example\n\n"
        << "Usage:\n"
        << "  ./example_logger [options]\n\n"
        << "Options:\n"
        << "  --log-level <level>   trace | debug | info | warn | error | critical | off\n"
        << "  --log-file <file>     save logs to file\n"
        << "  --debug               shortcut for --log-level debug\n"
        << "  --quiet               shortcut for --log-level error\n"
        << "  --help                show this help\n";
}

int main(int argc, char** argv)
{
    OpenCAX::LogLevel log_level = OpenCAX::LogLevel::Info;
    std::string log_file;
    bool use_file_log = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help") {
            printHelp();
            return 0;
        }
        else if (arg == "--debug") {
            log_level = OpenCAX::LogLevel::Debug;
        }
        else if (arg == "--quiet") {
            log_level = OpenCAX::LogLevel::Error;
        }
        else if (arg == "--log-level" && i + 1 < argc) {
            log_level = parseLogLevel(argv[++i]);
        }
        else if (arg == "--log-file" && i + 1 < argc) {
            log_file = argv[++i];
            use_file_log = true;
        }
    }

    if (use_file_log) {
        OpenCAX::Logger::init(log_level, log_file);
    } else {
        OpenCAX::Logger::init(log_level, "");
    }

    OpenCAX_LOG_INFO(OpenCAX::LogModule::Core, "OpenCAX started");
    OpenCAX_LOG_DEBUG(OpenCAX::LogModule::Core, "version: {}", OpenCAX_VERSION_STRING);

    OpenCAX_LOG_INFO(OpenCAX::LogModule::Meshing, "Start mesh generation");
    OpenCAX_LOG_INFO(OpenCAX::LogModule::Meshing, "mesh size min = {}, max = {}", 0.01, 0.1);

    OpenCAX_LOG_WARN(OpenCAX::LogModule::IO, "File not found: {}", "model.step");

    OpenCAX_LOG_ERROR(
        OpenCAX::LogModule::Solver,
        "Solver failed, code = {}",
        -1
    );

    OpenCAX::Logger::shutdown();

    return 0;
}