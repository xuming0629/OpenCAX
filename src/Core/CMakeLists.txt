add_library(OpenCAXCore
    Logger.cpp
)

add_library(OpenCAX::Core ALIAS OpenCAXCore)

target_include_directories(OpenCAXCore
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_features(OpenCAXCore PUBLIC cxx_std_17)