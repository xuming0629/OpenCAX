#include <gtest/gtest.h>

#include <OpenCAX/Geometry/ShapeFactory.h>
#include <OpenCAX/IO/OccIO.h>

#include <filesystem>

TEST(IO, WriteAndReadSTEP)
{
    auto shape =
        OpenCAX::ShapeFactory::makeBox(1,1,1);

    const std::string filename =
        "unit_test_box.step";

    bool ok =
        OpenCAX::OccIO::writeSTEP(
            shape,
            filename
        );

    EXPECT_TRUE(ok);

    EXPECT_TRUE(
        std::filesystem::exists(filename)
    );

    auto loaded =
        OpenCAX::OccIO::readSTEP(filename);

    EXPECT_FALSE(loaded.IsNull());

    std::filesystem::remove(filename);
}