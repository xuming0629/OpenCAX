#pragma once

#include <TopoDS_Shape.hxx>
#include <string>

namespace OpenCAX
{

class OccIO
{
public:
    static TopoDS_Shape readSTEP(const std::string& filename);
    static TopoDS_Shape readIGES(const std::string& filename);
    static TopoDS_Shape readBREP(const std::string& filename);
    static TopoDS_Shape readSTL(const std::string& filename);

    static bool writeSTEP(const TopoDS_Shape& shape, const std::string& filename);
    static bool writeBREP(const TopoDS_Shape& shape, const std::string& filename);
    static bool writeSTL(const TopoDS_Shape& shape, const std::string& filename);
};

}