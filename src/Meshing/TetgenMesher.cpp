#include <OpenCAX/Core/LogMacros.h>
#include <OpenCAX/Meshing/TetgenMesher.h>
#include <OpenCAX/IO/OccIO.h>


#include <tetgen.h>

#include <BRep_Tool.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>

#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopLoc_Location.hxx>
#include <Poly_Triangulation.hxx>
#include <Poly_Triangle.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>

#include <array>
#include <cmath>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace OpenCAX
{

namespace
{

static std::string toLower(const std::string& s)
{
    std::string out = s;
    for (char& c : out) {
        c = static_cast<char>(std::tolower(c));
    }
    return out;
}

static bool endsWith(const std::string& s, const std::string& suffix)
{
    if (s.size() < suffix.size()) {
        return false;
    }

    return s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

struct PointKey
{
    long long x;
    long long y;
    long long z;

    bool operator==(const PointKey& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct PointKeyHash
{
    std::size_t operator()(const PointKey& k) const
    {
        std::size_t h1 = std::hash<long long>{}(k.x);
        std::size_t h2 = std::hash<long long>{}(k.y);
        std::size_t h3 = std::hash<long long>{}(k.z);

        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

static PointKey makePointKey(double x, double y, double z, double eps = 1.0e-9)
{
    return PointKey{
        static_cast<long long>(std::llround(x / eps)),
        static_cast<long long>(std::llround(y / eps)),
        static_cast<long long>(std::llround(z / eps))
    };
}

} // namespace

Mesh TetgenMesher::generateVolumeMesh(
    const TopoDS_Shape& shape,
    const MeshingOptions& options
)
{
    Mesh mesh;

    if (shape.IsNull()) {
        OpenCAX_LOG_ERROR(
    LogModule::Meshing,
    "TetgenMesher input shape is null."
);
        return mesh;
    }

    std::vector<std::array<double, 3>> points;
    std::vector<std::array<int, 3>> triangles;

    double deflection = options.mesh_size_min;
    if (deflection <= 0.0) {
        deflection = 0.01;
    }

    if (!shapeToSurfaceTriangles(shape, points, triangles, deflection)) {
        OpenCAX_LOG_ERROR(
    LogModule::Meshing,
    "TetgenMesher failed to extract surface triangles."
);
        return mesh;
    }

    return generateFromSurfaceTriangles(points, triangles, options);
}

Mesh TetgenMesher::generateVolumeMeshFromFile(
    const std::string& filename,
    const MeshingOptions& options
)
{
    TopoDS_Shape shape;

    std::string lower = toLower(filename);

    if (endsWith(lower, ".step") || endsWith(lower, ".stp")) {
        shape = OccIO::readSTEP(filename);
    } else if (endsWith(lower, ".iges") || endsWith(lower, ".igs")) {
        shape = OccIO::readIGES(filename);
    } else if (endsWith(lower, ".brep")) {
        shape = OccIO::readBREP(filename);
    } else if (endsWith(lower, ".stl")) {
        shape = OccIO::readSTL(filename);
    } else {
        OpenCAX_LOG_WARN(
    LogModule::Meshing,
    "TetgenMesher unsupported file type: {}",
    filename
);
        return Mesh{};
    }

    if (shape.IsNull()) {
        OpenCAX_LOG_ERROR(
    LogModule::Meshing,
    "TetgenMesher failed to read file: {}",
    filename
);
        return Mesh{};
    }

    return generateVolumeMesh(shape, options);
}

Mesh TetgenMesher::createBox(
    double lx,
    double ly,
    double lz,
    const MeshingOptions& options
)
{
    TopoDS_Shape shape = BRepPrimAPI_MakeBox(lx, ly, lz).Shape();
    return generateVolumeMesh(shape, options);
}

Mesh TetgenMesher::createSphere(
    double radius,
    const MeshingOptions& options
)
{
    TopoDS_Shape shape = BRepPrimAPI_MakeSphere(radius).Shape();
    return generateVolumeMesh(shape, options);
}

Mesh TetgenMesher::createCylinder(
    double radius,
    double height,
    const MeshingOptions& options
)
{
    TopoDS_Shape shape = BRepPrimAPI_MakeCylinder(radius, height).Shape();
    return generateVolumeMesh(shape, options);
}

bool TetgenMesher::shapeToSurfaceTriangles(
    const TopoDS_Shape& shape,
    std::vector<std::array<double, 3>>& points,
    std::vector<std::array<int, 3>>& triangles,
    double deflection
)
{
    points.clear();
    triangles.clear();

    BRepMesh_IncrementalMesh mesher(shape, deflection);

    std::unordered_map<PointKey, int, PointKeyHash> point_map;

    auto addPoint = [&](const gp_Pnt& p) -> int {
        PointKey key = makePointKey(p.X(), p.Y(), p.Z());

        auto it = point_map.find(key);
        if (it != point_map.end()) {
            return it->second;
        }

        int id = static_cast<int>(points.size());
        points.push_back({p.X(), p.Y(), p.Z()});
        point_map[key] = id;
        return id;
    };

    for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next()) {
        TopoDS_Face face = TopoDS::Face(exp.Current());

        TopLoc_Location loc;
        Handle(Poly_Triangulation) triangulation =
            BRep_Tool::Triangulation(face, loc);

        if (triangulation.IsNull()) {
            continue;
        }

        const gp_Trsf& trsf = loc.Transformation();

        for (int i = 1; i <= triangulation->NbTriangles(); ++i) {
            int n1;
            int n2;
            int n3;

            triangulation->Triangle(i).Get(n1, n2, n3);

            gp_Pnt p1 = triangulation->Node(n1).Transformed(trsf);
            gp_Pnt p2 = triangulation->Node(n2).Transformed(trsf);
            gp_Pnt p3 = triangulation->Node(n3).Transformed(trsf);

            int id1 = addPoint(p1);
            int id2 = addPoint(p2);
            int id3 = addPoint(p3);

            if (id1 == id2 || id2 == id3 || id1 == id3) {
                continue;
            }

            if (face.Orientation() == TopAbs_REVERSED) {
                triangles.push_back({id1, id3, id2});
            } else {
                triangles.push_back({id1, id2, id3});
            }
        }
    }

    return !points.empty() && !triangles.empty();
}

Mesh TetgenMesher::generateFromSurfaceTriangles(
    const std::vector<std::array<double, 3>>& points,
    const std::vector<std::array<int, 3>>& triangles,
    const MeshingOptions& options
)
{
    Mesh mesh;

    if (points.empty() || triangles.empty()) {
        OpenCAX_LOG_ERROR(
            LogModule::Meshing,
            "TetgenMesher empty input surface."
        );
        return mesh;
    }

    tetgenio in;
    tetgenio out;

    in.firstnumber = 0;

    in.numberofpoints = static_cast<int>(points.size());
    in.pointlist = new REAL[in.numberofpoints * 3];

    for (int i = 0; i < in.numberofpoints; ++i) {
        in.pointlist[i * 3 + 0] = points[i][0];
        in.pointlist[i * 3 + 1] = points[i][1];
        in.pointlist[i * 3 + 2] = points[i][2];
    }

    in.numberoffacets = static_cast<int>(triangles.size());
    in.facetlist = new tetgenio::facet[in.numberoffacets];
    in.facetmarkerlist = new int[in.numberoffacets];

    for (int i = 0; i < in.numberoffacets; ++i) {
        tetgenio::facet& facet = in.facetlist[i];

        facet.numberofpolygons = 1;
        facet.polygonlist = new tetgenio::polygon[1];
        facet.numberofholes = 0;
        facet.holelist = nullptr;

        tetgenio::polygon& polygon = facet.polygonlist[0];
        polygon.numberofvertices = 3;
        polygon.vertexlist = new int[3];

        polygon.vertexlist[0] = triangles[i][0];
        polygon.vertexlist[1] = triangles[i][1];
        polygon.vertexlist[2] = triangles[i][2];

        in.facetmarkerlist[i] = 1;
    }

    std::ostringstream switches;
    switches << "p";

    if (options.tetgen_quality > 0.0) {
        switches << "q" << options.tetgen_quality;
    }

    if (options.tetgen_max_volume > 0.0) {
        switches << "a" << options.tetgen_max_volume;
    }

    if (!options.verbose) {
        switches << "Q";
    }

    std::string switch_string = switches.str();

    tetgenbehavior behavior;

    std::vector<char> switch_buffer(
        switch_string.begin(),
        switch_string.end()
    );
    switch_buffer.push_back('\0');

    if (!behavior.parse_commandline(switch_buffer.data())) {
        OpenCAX_LOG_ERROR(
            LogModule::Meshing,
            "TetgenMesher failed to parse TetGen switches: {}",
            switch_string
        );
        return mesh;
    }

    try {
        tetrahedralize(
            &behavior,
            &in,
            &out
        );
    } catch (...) {
        OpenCAX_LOG_ERROR(
            LogModule::Meshing,
            "TetgenMesher tetrahedralize failed."
        );
        return mesh;
    }

    mesh.info().topology_dim = TopologyDim::Dim3;
    mesh.info().geometry_dim = GeometryDim::Dim3;
    mesh.info().kind = MeshKind::Volume;
    mesh.info().structure = MeshStructure::Unstructured;
    mesh.info().is_second_order = false;

    for (int i = 0; i < out.numberofpoints; ++i) {
        mesh.add_node(
            out.pointlist[i * 3 + 0],
            out.pointlist[i * 3 + 1],
            out.pointlist[i * 3 + 2]
        );
    }

    for (int i = 0; i < out.numberoftetrahedra; ++i) {
        mesh.add_cell(
            CellType::Tetra4,
            {
                out.tetrahedronlist[i * 4 + 0],
                out.tetrahedronlist[i * 4 + 1],
                out.tetrahedronlist[i * 4 + 2],
                out.tetrahedronlist[i * 4 + 3]
            }
        );
    }

    OpenCAX_LOG_INFO(
        LogModule::Meshing,
        "TetgenMesher generated mesh: nodes={}, tetra={}",
        mesh.num_nodes(),
        mesh.num_cells()
    );

    return mesh;
}

} // namespace OpenCAX