#include <OpenCAX/Mesh/Mesh.h>

#include <gtest/gtest.h>

TEST(MeshBasic, EmptyMesh)
{
    OpenCAX::Mesh mesh;

    EXPECT_EQ(mesh.num_nodes(), 0);
    EXPECT_EQ(mesh.num_cells(), 0);
}