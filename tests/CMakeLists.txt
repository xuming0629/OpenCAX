find_package(GTest REQUIRED)

include(GoogleTest)

# =====================================
# test_geometry_box
# =====================================

add_executable(test_geometry_box
    test_geometry_box.cpp
)

target_link_libraries(test_geometry_box
    PRIVATE
        GTest::gtest_main
        OpenCAX::Core
        OpenCAX::Geometry
)

gtest_discover_tests(test_geometry_box)


# =====================================
# test_io_step
# =====================================

add_executable(test_io_step
    test_io_step.cpp
)

target_link_libraries(test_io_step
    PRIVATE
        GTest::gtest_main
        OpenCAX::Core
        OpenCAX::Geometry
        OpenCAX::IO
)

gtest_discover_tests(test_io_step)


# # =====================================
# # test_meshing
# # =====================================

# add_executable(test_meshing
#     test_meshing.cpp
# )

# target_link_libraries(test_meshing
#     PRIVATE
#         GTest::gtest_main
#         OpenCAX::Core
#         OpenCAX::Geometry
#         OpenCAX::Meshing
# )

# gtest_discover_tests(test_meshing)