#include <OpenCAX/Meshing/StructuredMeshGenerator.h>

#include <cmath>
#include <stdexcept>

namespace OpenCAX
{

static Vec3 sub(const Vec3& a, const Vec3& b)
{
    return {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

static Vec3 cross(const Vec3& a, const Vec3& b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

static double dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static double norm(const Vec3& v)
{
    return std::sqrt(dot(v, v));
}

static bool is_coplanar_quad(
    const Vec3& p00,
    const Vec3& p10,
    const Vec3& p11,
    const Vec3& p01,
    double tolerance
)
{
    Vec3 v1 = sub(p10, p00);
    Vec3 v2 = sub(p01, p00);
    Vec3 v3 = sub(p11, p00);

    Vec3 n = cross(v1, v2);
    double n_norm = norm(n);

    if (n_norm < tolerance) {
        return false;
    }

    double distance = std::abs(dot(n, v3)) / n_norm;

    return distance <= tolerance;
}

static Vec3 bilinear_interpolate(
    const Vec3& p00,
    const Vec3& p10,
    const Vec3& p11,
    const Vec3& p01,
    double u,
    double v
)
{
    Vec3 p;

    p.x = (1.0 - u) * (1.0 - v) * p00.x
        + u * (1.0 - v) * p10.x
        + u * v * p11.x
        + (1.0 - u) * v * p01.x;

    p.y = (1.0 - u) * (1.0 - v) * p00.y
        + u * (1.0 - v) * p10.y
        + u * v * p11.y
        + (1.0 - u) * v * p01.y;

    p.z = (1.0 - u) * (1.0 - v) * p00.z
        + u * (1.0 - v) * p10.z
        + u * v * p11.z
        + (1.0 - u) * v * p01.z;

    return p;
}

static Vec3 trilinear_interpolate(
    const Vec3& p000,
    const Vec3& p100,
    const Vec3& p110,
    const Vec3& p010,
    const Vec3& p001,
    const Vec3& p101,
    const Vec3& p111,
    const Vec3& p011,
    double u,
    double v,
    double w
)
{
    Vec3 p;

    double N000 = (1.0 - u) * (1.0 - v) * (1.0 - w);
    double N100 = u * (1.0 - v) * (1.0 - w);
    double N110 = u * v * (1.0 - w);
    double N010 = (1.0 - u) * v * (1.0 - w);

    double N001 = (1.0 - u) * (1.0 - v) * w;
    double N101 = u * (1.0 - v) * w;
    double N111 = u * v * w;
    double N011 = (1.0 - u) * v * w;

    p.x = N000 * p000.x
        + N100 * p100.x
        + N110 * p110.x
        + N010 * p010.x
        + N001 * p001.x
        + N101 * p101.x
        + N111 * p111.x
        + N011 * p011.x;

    p.y = N000 * p000.y
        + N100 * p100.y
        + N110 * p110.y
        + N010 * p010.y
        + N001 * p001.y
        + N101 * p101.y
        + N111 * p111.y
        + N011 * p011.y;

    p.z = N000 * p000.z
        + N100 * p100.z
        + N110 * p110.z
        + N010 * p010.z
        + N001 * p001.z
        + N101 * p101.z
        + N111 * p111.z
        + N011 * p011.z;

    return p;
}


Mesh StructuredMeshGenerator::create_line(
    const Vec3& p0,
    const Vec3& p1,
    int nx
)
{
    if (nx <= 0) {
        throw std::runtime_error("create_line: nx must be > 0");
    }

    Mesh mesh;

    mesh.info().topology_dim = TopologyDim::Dim1;
    mesh.info().geometry_dim = GeometryDim::Dim3;
    mesh.info().kind = MeshKind::Curve;
    mesh.info().structure = MeshStructure::Structured;

    for (int i = 0; i <= nx; ++i) {
        double t = static_cast<double>(i) / static_cast<double>(nx);

        double x = (1.0 - t) * p0.x + t * p1.x;
        double y = (1.0 - t) * p0.y + t * p1.y;
        double z = (1.0 - t) * p0.z + t * p1.z;

        mesh.add_node(x, y, z);
    }

    for (int i = 0; i < nx; ++i) {
        mesh.add_cell(CellType::Line2, {i, i + 1});
    }

    return mesh;
}

Mesh StructuredMeshGenerator::create_quad(
    const Vec3& p00,
    const Vec3& p10,
    const Vec3& p11,
    const Vec3& p01,
    int nx,
    int ny,
    double tolerance
)
{
    if (nx <= 0 || ny <= 0) {
        throw std::runtime_error("create_quad_plane: nx and ny must be > 0");
    }

    if (!is_coplanar_quad(p00, p10, p11, p01, tolerance)) {
        throw std::runtime_error(
            "create_quad_plane: input four points are not coplanar"
        );
    }

    Mesh mesh;

    mesh.info().topology_dim = TopologyDim::Dim2;
    mesh.info().geometry_dim = GeometryDim::Dim3;
    mesh.info().kind = MeshKind::Surface;
    mesh.info().structure = MeshStructure::Structured;

    for (int j = 0; j <= ny; ++j) {
        double v = static_cast<double>(j) / static_cast<double>(ny);

        for (int i = 0; i <= nx; ++i) {
            double u = static_cast<double>(i) / static_cast<double>(nx);

            Vec3 p = bilinear_interpolate(
                p00,
                p10,
                p11,
                p01,
                u,
                v
            );

            mesh.add_node(p.x, p.y, p.z);
        }
    }

    auto id = [nx](int i, int j) {
        return j * (nx + 1) + i;
    };

    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            int n0 = id(i, j);
            int n1 = id(i + 1, j);
            int n2 = id(i + 1, j + 1);
            int n3 = id(i, j + 1);

            mesh.add_cell(CellType::Quad4, {n0, n1, n2, n3});
        }
    }

    return mesh;
}

Mesh StructuredMeshGenerator::create_tri(
    const Vec3& p00,
    const Vec3& p10,
    const Vec3& p11,
    const Vec3& p01,
    int nx,
    int ny,
    double tolerance
)
{
    if (nx <= 0 || ny <= 0) {
        throw std::runtime_error("create_tri: nx and ny must be > 0");
    }

    if (!is_coplanar_quad(p00, p10, p11, p01, tolerance)) {
        throw std::runtime_error(
            "create_tri: input four points are not coplanar"
        );
    }

    Mesh mesh;

    mesh.info().topology_dim = TopologyDim::Dim2;
    mesh.info().geometry_dim = GeometryDim::Dim3;
    mesh.info().kind = MeshKind::Surface;
    mesh.info().structure = MeshStructure::Structured;

    // ==========================================
    // generate nodes
    // ==========================================

    for (int j = 0; j <= ny; ++j) {
        double v = static_cast<double>(j) / static_cast<double>(ny);

        for (int i = 0; i <= nx; ++i) {
            double u = static_cast<double>(i) / static_cast<double>(nx);

            Vec3 p = bilinear_interpolate(
                p00,
                p10,
                p11,
                p01,
                u,
                v
            );

            mesh.add_node(p.x, p.y, p.z);
        }
    }

    auto id = [nx](int i, int j) {
        return j * (nx + 1) + i;
    };

    // ==========================================
    // split each quad into two triangles
    //
    // n3 ---- n2
    // |    /  |
    // |   /   |
    // n0 ---- n1
    // ==========================================

    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            int n0 = id(i, j);
            int n1 = id(i + 1, j);
            int n2 = id(i + 1, j + 1);
            int n3 = id(i, j + 1);

            mesh.add_cell(CellType::Triangle3, {n0, n1, n2});
            mesh.add_cell(CellType::Triangle3, {n0, n2, n3});
        }
    }

    return mesh;
}

Mesh StructuredMeshGenerator::create_hex(
    const Vec3& p000,
    const Vec3& p100,
    const Vec3& p110,
    const Vec3& p010,
    const Vec3& p001,
    const Vec3& p101,
    const Vec3& p111,
    const Vec3& p011,
    int nx,
    int ny,
    int nz
)
{
    if (nx <= 0 || ny <= 0 || nz <= 0) {
        throw std::runtime_error(
            "create_hex: nx, ny and nz must be > 0"
        );
    }

    Mesh mesh;

    mesh.info().topology_dim = TopologyDim::Dim3;
    mesh.info().geometry_dim = GeometryDim::Dim3;
    mesh.info().kind = MeshKind::Volume;
    mesh.info().structure = MeshStructure::Structured;

    // ==========================================
    // nodes
    // ==========================================

    for (int k = 0; k <= nz; ++k) {
        double w = static_cast<double>(k) / static_cast<double>(nz);

        for (int j = 0; j <= ny; ++j) {
            double v = static_cast<double>(j) / static_cast<double>(ny);

            for (int i = 0; i <= nx; ++i) {
                double u = static_cast<double>(i) / static_cast<double>(nx);

                Vec3 p = trilinear_interpolate(
                    p000,
                    p100,
                    p110,
                    p010,
                    p001,
                    p101,
                    p111,
                    p011,
                    u,
                    v,
                    w
                );

                mesh.add_node(p.x, p.y, p.z);
            }
        }
    }

    auto id = [nx, ny](int i, int j, int k) {
        return k * (ny + 1) * (nx + 1)
             + j * (nx + 1)
             + i;
    };

    // ==========================================
    // cells
    // ==========================================

    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                int n000 = id(i,     j,     k);
                int n100 = id(i + 1, j,     k);
                int n110 = id(i + 1, j + 1, k);
                int n010 = id(i,     j + 1, k);

                int n001 = id(i,     j,     k + 1);
                int n101 = id(i + 1, j,     k + 1);
                int n111 = id(i + 1, j + 1, k + 1);
                int n011 = id(i,     j + 1, k + 1);

                mesh.add_cell(CellType::Hexa8, {
                    n000,
                    n100,
                    n110,
                    n010,
                    n001,
                    n101,
                    n111,
                    n011
                });
            }
        }
    }

    return mesh;
}


Mesh StructuredMeshGenerator::create_tet(
    const Vec3& p000,
    const Vec3& p100,
    const Vec3& p110,
    const Vec3& p010,
    const Vec3& p001,
    const Vec3& p101,
    const Vec3& p111,
    const Vec3& p011,
    int nx,
    int ny,
    int nz
)
{
    if (nx <= 0 || ny <= 0 || nz <= 0) {
        throw std::runtime_error(
            "create_tet: nx, ny and nz must be > 0"
        );
    }

    Mesh mesh;

    mesh.info().topology_dim = TopologyDim::Dim3;
    mesh.info().geometry_dim = GeometryDim::Dim3;
    mesh.info().kind = MeshKind::Volume;
    mesh.info().structure = MeshStructure::Structured;

    // 1. nodes
    for (int k = 0; k <= nz; ++k) {
        double w = static_cast<double>(k) / static_cast<double>(nz);

        for (int j = 0; j <= ny; ++j) {
            double v = static_cast<double>(j) / static_cast<double>(ny);

            for (int i = 0; i <= nx; ++i) {
                double u = static_cast<double>(i) / static_cast<double>(nx);

                Vec3 p = trilinear_interpolate(
                    p000, p100, p110, p010,
                    p001, p101, p111, p011,
                    u, v, w
                );

                mesh.add_node(p.x, p.y, p.z);
            }
        }
    }

    auto id = [nx, ny](int i, int j, int k) {
        return k * (ny + 1) * (nx + 1)
             + j * (nx + 1)
             + i;
    };

    // 2. cells: 每个 Hexa8 拆成 6 个 Tetra4
    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                int n000 = id(i,     j,     k);
                int n100 = id(i + 1, j,     k);
                int n110 = id(i + 1, j + 1, k);
                int n010 = id(i,     j + 1, k);

                int n001 = id(i,     j,     k + 1);
                int n101 = id(i + 1, j,     k + 1);
                int n111 = id(i + 1, j + 1, k + 1);
                int n011 = id(i,     j + 1, k + 1);

                // 共用主对角线 n000 -> n111
                mesh.add_cell(CellType::Tetra4, {n000, n100, n110, n111});
                mesh.add_cell(CellType::Tetra4, {n000, n110, n010, n111});
                mesh.add_cell(CellType::Tetra4, {n000, n010, n011, n111});
                mesh.add_cell(CellType::Tetra4, {n000, n011, n001, n111});
                mesh.add_cell(CellType::Tetra4, {n000, n001, n101, n111});
                mesh.add_cell(CellType::Tetra4, {n000, n101, n100, n111});
            }
        }
    }

    return mesh;
}

}