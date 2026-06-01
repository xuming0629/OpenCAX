#include <OpenCAX/IO/OccIO.h>
#include <OpenCAX/Core/LogMacros.h>

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
        OpenCAX_LOG_ERROR(
            LogModule::IO,
            "Failed to read STEP: {}",
            filename
        );
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
        OpenCAX_LOG_ERROR(
            LogModule::IO,
            "Failed to read IGES: {}",
            filename
        );
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
        OpenCAX_LOG_ERROR(
            LogModule::IO,
            "Failed to read BREP: {}",
            filename
        );
        return {};
    }

    return shape;
}

TopoDS_Shape OccIO::readSTL(const std::string& filename)
{
    TopoDS_Shape shape;
    StlAPI_Reader reader;

    reader.Read(shape, filename.c_str());

    if (shape.IsNull()) {
        OpenCAX_LOG_ERROR(
            LogModule::IO,
            "Failed to read STL: {}",
            filename
        );
    }

    return shape;
}

bool OccIO::writeSTEP(const TopoDS_Shape& shape, const std::string& filename)
{
    STEPControl_Writer writer;
    writer.Transfer(shape, STEPControl_AsIs);

    bool ok = writer.Write(filename.c_str()) == IFSelect_RetDone;

    if (!ok) {
        OpenCAX_LOG_ERROR(
            LogModule::IO,
            "Failed to write STEP: {}",
            filename
        );
    }

    return ok;
}

bool OccIO::writeBREP(const TopoDS_Shape& shape, const std::string& filename)
{
    bool ok = BRepTools::Write(shape, filename.c_str());

    if (!ok) {
        OpenCAX_LOG_ERROR(
            LogModule::IO,
            "Failed to write BREP: {}",
            filename
        );
    }

    return ok;
}

bool OccIO::writeSTL(const TopoDS_Shape& shape, const std::string& filename)
{
    StlAPI_Writer writer;
    writer.ASCIIMode() = true;

    writer.Write(shape, filename.c_str());

    OpenCAX_LOG_INFO(
        LogModule::IO,
        "Write STL: {}",
        filename
    );

    return true;
}

} // namespace OpenCAX