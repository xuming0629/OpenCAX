find_package(Doxygen REQUIRED)

set(OPEN_CAX_DOC_INPUT_DIR
    "${CMAKE_INSTALL_PREFIX}/include"
)

set(OPEN_CAX_DOC_OUTPUT_DIR
    "${CMAKE_BINARY_DIR}/docs"
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    @ONLY
)

add_custom_target(OpenCAXDocs
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    COMMENT "Generating OpenCAX API documentation from installed headers"
    VERBATIM
)

install(CODE "
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target OpenCAXDocs
    )
")

install(DIRECTORY
    ${OPEN_CAX_DOC_OUTPUT_DIR}/html
    DESTINATION share/doc/OpenCAX
)