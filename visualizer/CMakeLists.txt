FILE(GLOB CPPSources *.cpp)
FILE(GLOB HPPSources *.h)
add_library(visualizer ${CPPSources} ${HPPSOURCES})
target_include_directories (visualizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(visualizer
    PUBLIC cxx_lambdas cxx_auto_type
    PRIVATE cxx_lambdas cxx_auto_type)
