find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(opencax bindings.cpp)

target_link_libraries(opencax PRIVATE OpenCAX::Core)