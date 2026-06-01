#include <gtest/gtest.h>

#include <OpenCAX/Geometry/ShapeFactory.h>
#include <OpenCAX/Meshing/GmshMesher.h>

TEST(Meshing, GenerateTetMesh)
{
    auto shape =
        OpenCAX::ShapeFactory::makeBox(
            1.0,
            1.0,
            1.0
        );

    OpenCAX::MeshingOptions options;

    options.mesh_size_min = 0.2;
    options.mesh_size_max = 0.2;

    auto mesh =
        OpenCAX::GmshMesher::generateVolumeMesh(
            shape,
            options
        );

    EXPECT_GT(mesh.nodes().size(), 0);

    EXPECT_GT(mesh.cells().size(), 0);
}