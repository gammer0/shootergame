cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_CXX_STANDARD 11)
set(project_name SDLshooter)
project(${project_name} VERSION 0.1.0 LANGUAGES C CXX)



find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

file(GLOB SOURCES_CXX
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(${project_name} ${SOURCES_CXX})

target_link_libraries(${project_name} 
                        SDL2::SDL2
                        SDL2_image::SDL2_image
                        SDL2_ttf::SDL2_ttf
                        SDL2_mixer::SDL2_mixer)