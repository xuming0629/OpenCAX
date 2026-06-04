#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <string>
#include <vector>

namespace OpenCAX
{

struct ShapeCheckResult
{
    bool valid = true;
    std::vector<std::string> messages;
};

class OpenCAX_API ShapeCheck
{
public:
    static ShapeCheckResult check(const Shape& shape);

    static bool isValid(const Shape& shape);
};

} // namespace OpenCAX