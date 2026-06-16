#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <string>

namespace OpenCAX
{

/**
 * @brief 几何文件读写工具类
 */
class OpenCAX_API GeometryIO
{
public:
    static Shape readSTEP(const std::string& filename);
    static bool writeSTEP(const Shape& shape, const std::string& filename);

    static Shape readIGES(const std::string& filename);
    static bool writeIGES(const Shape& shape, const std::string& filename);

    static Shape readBREP(const std::string& filename);
    static bool writeBREP(const Shape& shape, const std::string& filename);

    static Shape readSTL(const std::string& filename);
    static bool writeSTL(const Shape& shape, const std::string& filename);
};

} // namespace OpenCAX
