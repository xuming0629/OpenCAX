#include <OpenCAX/IO/OccIO.h>
#include <OpenCAX/Core/Logger.h>

#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <IGESControl_Reader.hxx>
#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <StlAPI_Reader.hxx>
#include <StlAPI_Writer.hxx>

namespace OpenCAX
{

TopoDS_Shape OccIO::readSTEP(const std::string& filename)
{
    STEPControl_Reader reader;
    IFSelect_ReturnStatus status = reader.ReadFile(filename.c_str());

    if (status != IFSelect_RetDone) {
        Logger::error("Failed to read STEP: " + filename);
        return {};
    }

    reader.TransferRoots();
    return reader.OneShape();
}

TopoDS_Shape OccIO::readIGES(const std::string& filename)
{
    IGESControl_Reader reader;
    IFSelect_ReturnStatus status = reader.ReadFile(filename.c_str());

    if (status != IFSelect_RetDone) {
        Logger::error("Failed to read IGES: " + filename);
        return {};
    }

    reader.TransferRoots();
    return reader.OneShape();
}

TopoDS_Shape OccIO::readBREP(const std::string& filename)
{
    TopoDS_Shape shape;
    BRep_Builder builder;

    if (!BRepTools::Read(shape, filename.c_str(), builder)) {
        Logger::error("Failed to read BREP: " + filename);
        return {};
    }

    return shape;
}

TopoDS_Shape OccIO::readSTL(const std::string& filename)
{
    TopoDS_Shape shape;
    StlAPI_Reader reader;
    reader.Read(shape, filename.c_str());
    return shape;
}

bool OccIO::writeSTEP(const TopoDS_Shape& shape, const std::string& filename)
{
    STEPControl_Writer writer;
    writer.Transfer(shape, STEPControl_AsIs);

    return writer.Write(filename.c_str()) == IFSelect_RetDone;
}

bool OccIO::writeBREP(const TopoDS_Shape& shape, const std::string& filename)
{
    return BRepTools::Write(shape, filename.c_str());
}

bool OccIO::writeSTL(const TopoDS_Shape& shape, const std::string& filename)
{
    StlAPI_Writer writer;
    writer.ASCIIMode() = true;
    writer.Write(shape, filename.c_str());
    return true;
}

}