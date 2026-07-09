#include <OpenCAX/Meshing/Gmsh/GmshMeshGenerator.h>
#include <OpenCAX/Meshing/Gmsh/GmshMeshReader.h>
#include <OpenCAX/Meshing/HighOrder/MeshOrderConverter.h>

#ifdef OPEN_CAX_HAS_GMSH
#include <gmsh.h>
#endif

#include <stdexcept>
#include <unordered_map>

namespace OpenCAX
{

static void require_gmsh()
{
#ifndef OPEN_CAX_HAS_GMSH
    throw std::runtime_error("GmshMeshGenerator requires OPEN_CAX_HAS_GMSH.");
#endif
}

TriangleMesh GmshMeshGenerator::rectangle_2d(double xmin,double xmax,double ymin,double ymax,const MeshGenerationOptions& options)
{
#ifndef OPEN_CAX_HAS_GMSH
    (void)xmin;(void)xmax;(void)ymin;(void)ymax;(void)options;
    require_gmsh();
    return TriangleMesh{};
#else
    if(!gmsh::isInitialized()) gmsh::initialize();
    gmsh::clear();
    gmsh::option::setNumber("General.Terminal", options.verbose ? 1.0 : 0.0);
    gmsh::model::add(options.model_name);

    int p1=gmsh::model::geo::addPoint(xmin,ymin,0,options.element_size);
    int p2=gmsh::model::geo::addPoint(xmax,ymin,0,options.element_size);
    int p3=gmsh::model::geo::addPoint(xmax,ymax,0,options.element_size);
    int p4=gmsh::model::geo::addPoint(xmin,ymax,0,options.element_size);
    int l1=gmsh::model::geo::addLine(p1,p2), l2=gmsh::model::geo::addLine(p2,p3), l3=gmsh::model::geo::addLine(p3,p4), l4=gmsh::model::geo::addLine(p4,p1);
    int loop=gmsh::model::geo::addCurveLoop({l1,l2,l3,l4});
    gmsh::model::geo::addPlaneSurface({loop});
    gmsh::model::geo::synchronize();
    gmsh::option::setNumber("Mesh.Algorithm", static_cast<double>(options.algorithm_2d));
    gmsh::model::mesh::setOrder(options.order);
    gmsh::model::mesh::generate(2);
    if(options.optimize) gmsh::model::mesh::optimize("Netgen");

    std::string temp="opencax_gmsh_rectangle_tmp.msh";
    gmsh::write(temp);
    TriangleMesh mesh=GmshMeshReader::read_triangle_mesh(temp);
    mesh.set_name("GmshRectangleTriangleMesh");
    return mesh;
#endif
}

TriangleMesh GmshMeshGenerator::structured_rectangle_2d(double xmin,double xmax,double ymin,double ymax,int nx,int ny,const MeshGenerationOptions& options)
{
#ifndef OPEN_CAX_HAS_GMSH
    (void)xmin;(void)xmax;(void)ymin;(void)ymax;(void)nx;(void)ny;(void)options;
    require_gmsh();
    return TriangleMesh{};
#else
    MeshGenerationOptions opt=options;
    if(opt.order==2 || opt.cell_type==CellType::Triangle6)
        return MeshOrderConverter::triangle3_to_triangle6(TriangleMesh::create_structured_rectangle(xmin,xmax,ymin,ymax,nx,ny,opt.alternate_diagonal));
    return TriangleMesh::create_structured_rectangle(xmin,xmax,ymin,ymax,nx,ny,opt.alternate_diagonal);
#endif
}

void GmshMeshGenerator::finalize()
{
#ifdef OPEN_CAX_HAS_GMSH
    if(gmsh::isInitialized()) gmsh::finalize();
#endif
}

} // namespace OpenCAX
