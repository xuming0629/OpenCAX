#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/ConvergenceStudy2D.h>

#include <string>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API ConvergencePlotter
{
public:
    static bool writePythonScript(
        const std::string& csv_file,
        const std::string& py_file,
        const std::string& png_file
    );
};

} // namespace OpenCAX