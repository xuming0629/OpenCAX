#include <OpenCAX/Core/Logger.h>
#include <OpenCAX/Core/LogMacros.h>

int main()
{
    OpenCAX::Logger::init(
        OpenCAX::LogLevel::Debug,
        "logs/OpenCAX.log"
    );

    OpenCAX_LOG_INFO(OpenCAX::LogModule::Core, "OpenCAX started");
    OpenCAX_LOG_DEBUG(OpenCAX::LogModule::Core, "version: {}", OpenCAX_VERSION_STRING);
    OpenCAX_LOG_INFO(OpenCAX::LogModule::Meshing, "Start mesh generation");
    OpenCAX_LOG_WARN(OpenCAX::LogModule::IO, "File not found: {}", "model.step");
    OpenCAX_LOG_ERROR(
        OpenCAX::LogModule::Solver,
        "Solver failed, code = {}",
        -1
    );

    OpenCAX::Logger::shutdown();

    return 0;
}