#include <OpenCAX/Geometry/GeometryIO.h>

#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESControl_Writer.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_StepModelType.hxx>
#include <STEPControl_Writer.hxx>
#include <StlAPI_Reader.hxx>
#include <StlAPI_Writer.hxx>

#include <stdexcept>

namespace OpenCAX
{

namespace
{
void checkShape(const Shape& shape, const char* message)
{
    if (shape.isNull()) {
        throw std::runtime_error(message);
    }
}
}

Shape GeometryIO::readSTEP(const std::string& filename)
{
    STEPControl_Reader reader;
    IFSelect_ReturnStatus status = reader.ReadFile(filename.c_str());

    if (status != IFSelect_RetDone) {
        throw std::runtime_error("Failed to read STEP file: " + filename);
    }

    reader.TransferRoots();
    TopoDS_Shape shape = reader.OneShape();

    if (shape.IsNull()) {
        throw std::runtime_error("STEP file contains null shape: " + filename);
    }

    return Shape(shape);
}

bool GeometryIO::writeSTEP(const Shape& shape, const std::string& filename)
{
    checkShape(shape, "writeSTEP input shape is null.");

    STEPControl_Writer writer;
    writer.Transfer(shape.occ(), STEPControl_AsIs);

    IFSelect_ReturnStatus status = writer.Write(filename.c_str());
    return status == IFSelect_RetDone;
}

Shape GeometryIO::readIGES(const std::string& filename)
{
    IGESControl_Reader reader;
    IFSelect_ReturnStatus status = reader.ReadFile(filename.c_str());

    if (status != IFSelect_RetDone) {
        throw std::runtime_error("Failed to read IGES file: " + filename);
    }

    reader.TransferRoots();
    TopoDS_Shape shape = reader.OneShape();

    if (shape.IsNull()) {
        throw std::runtime_error("IGES file contains null shape: " + filename);
    }

    return Shape(shape);
}

bool GeometryIO::writeIGES(const Shape& shape, const std::string& filename)
{
    checkShape(shape, "writeIGES input shape is null.");

    IGESControl_Writer writer;
    writer.AddShape(shape.occ());

    return writer.Write(filename.c_str()) == Standard_True;
}

Shape GeometryIO::readBREP(const std::string& filename)
{
    TopoDS_Shape shape;
    BRep_Builder builder;

    const Standard_Boolean ok = BRepTools::Read(shape, filename.c_str(), builder);

    if (!ok || shape.IsNull()) {
        throw std::runtime_error("Failed to read BREP file: " + filename);
    }

    return Shape(shape);
}

bool GeometryIO::writeBREP(const Shape& shape, const std::string& filename)
{
    checkShape(shape, "writeBREP input shape is null.");

    return BRepTools::Write(shape.occ(), filename.c_str()) == Standard_True;
}

Shape GeometryIO::readSTL(const std::string& filename)
{
    TopoDS_Shape shape;
    StlAPI_Reader reader;

    const Standard_Boolean ok = reader.Read(shape, filename.c_str());

    if (!ok || shape.IsNull()) {
        throw std::runtime_error("Failed to read STL file: " + filename);
    }

    return Shape(shape);
}

bool GeometryIO::writeSTL(const Shape& shape, const std::string& filename)
{
    checkShape(shape, "writeSTL input shape is null.");

    StlAPI_Writer writer;
    return writer.Write(shape.occ(), filename.c_str()) == Standard_True;
}

} // namespace OpenCAX
