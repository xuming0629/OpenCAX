#include <gtest/gtest.h>

#include <OpenCAX/Geometry/ShapeFactory.h>

#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>

TEST(Geometry, CreateBox)
{
    auto shape = OpenCAX::ShapeFactory::makeBox(
        1.0,
        2.0,
        3.0
    );

    Bnd_Box bbox;

    BRepBndLib::Add(shape, bbox);

    double xmin, ymin, zmin;
    double xmax, ymax, zmax;

    bbox.Get(
        xmin, ymin, zmin,
        xmax, ymax, zmax
    );

    EXPECT_NEAR(xmax - xmin, 1.0, 1e-6);
    EXPECT_NEAR(ymax - ymin, 2.0, 1e-6);
    EXPECT_NEAR(zmax - zmin, 3.0, 1e-6);
}