cmake_minimum_required(VERSION 3.5)
project(Bachelor)
find_package(Threads REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
  #set(CMAKE_BUILD_TYPE Release)
  set(CMAKE_BUILD_TYPE Release)
  SET( CMAKE_BUILD_TYPE Release ... FORCE )
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Os -s")


add_subdirectory(visualizer)
add_subdirectory(src)
add_executable(Bachelor main.cpp)
target_link_libraries(Bachelor visualizer src pthread)

add_custom_command(
    TARGET Bachelor
    POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        $<TARGET_FILE_DIR:Bachelor>/assets)





