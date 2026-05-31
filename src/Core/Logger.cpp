#include <OpenCAX/Core/Logger.h>

namespace OpenCAX
{

void Logger::info(const std::string& msg)
{
    std::cout << "[OpenCAX][Info] " << msg << std::endl;
}

void Logger::warn(const std::string& msg)
{
    std::cout << "[OpenCAX][Warn] " << msg << std::endl;
}

void Logger::error(const std::string& msg)
{
    std::cerr << "[OpenCAX][Error] " << msg << std::endl;
}

}