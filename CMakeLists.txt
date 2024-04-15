cmake_minimum_required(VERSION 3.15)
project(minty VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(-DSPDLOG_USE_STD_FORMAT 1)

find_package(fmt REQUIRED)
find_package(SDL2 REQUIRED CONFIG)
find_package(spdlog REQUIRED)

file(GLOB incs ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB srcs ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(minty ${incs} ${srcs})
target_include_directories(minty PUBLIC ${SDL2_INCLUDE_DIRS} ${fmt_INCLUDE_DIRS} ${spdlog_INCLUDE_DIRS})
target_link_libraries(minty PUBLIC spdlog::spdlog fmt::fmt SDL2::SDL2main SDL2::SDL2-static)
