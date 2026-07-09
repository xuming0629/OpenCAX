# ============================================================
# OpenCAX Post Recovery
# ============================================================

add_library(OpenCAXPostRecovery
    FieldRecovery2D.cpp
)

add_library(OpenCAX::PostRecovery ALIAS OpenCAXPostRecovery)

target_include_directories(OpenCAXPostRecovery
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(OpenCAXPostRecovery
    PUBLIC
        cxx_std_17
)

target_compile_definitions(OpenCAXPostRecovery
    PRIVATE
        OpenCAX_EXPORTS
)

target_link_libraries(OpenCAXPostRecovery
    PUBLIC
        OpenCAX::Core
        OpenCAX::Mesh
)

if(OPEN_CAX_USE_EIGEN3)
    target_link_libraries(OpenCAXPostRecovery
        PUBLIC
            Eigen3::Eigen
    )
else()
    message(FATAL_ERROR "OpenCAX PostRecovery requires Eigen3. Please enable OPEN_CAX_USE_EIGEN3=ON.")
endif()

set_target_properties(OpenCAXPostRecovery PROPERTIES
    OUTPUT_NAME OpenCAXPostRecovery
    EXPORT_NAME PostRecovery
)

install(TARGETS OpenCAXPostRecovery
    EXPORT OpenCAXTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/OpenCAX/Post/Recovery
    DESTINATION include/OpenCAX/Post
)