cmake_minimum_required(VERSION 3.15)
project(minty VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(fmt REQUIRED CONFIG)
find_package(SDL2 REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)
find_package(opengl_system REQUIRED CONFIG)
find_package(glm REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)

file(GLOB incs ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB srcs ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(minty ${incs} ${srcs})
target_include_directories(minty PUBLIC ${SDL2_INCLUDE_DIRS} ${fmt_INCLUDE_DIRS} ${spdlog_INCLUDE_DIRS} ${SDL2_image_INCLUDE_DIR} ${opengl_INCLUDE_DIR} ${glm_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(minty PUBLIC spdlog::spdlog fmt::fmt SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image opengl::opengl glm::glm)
