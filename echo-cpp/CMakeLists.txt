cmake_minimum_required(VERSION 3.20)
project(maelstrom_app VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

include(FetchContent)

FetchContent_Declare(
    maelstrom_node
    GIT_REPOSITORY https://github.com/astavonin/maelstrom-node.git
    GIT_TAG v1.0.0
)

FetchContent_MakeAvailable(maelstrom_node)

add_executable(maelstrom_app main.cpp)

target_link_libraries(maelstrom_app PRIVATE maelstrom_node)

find_package(spdlog REQUIRED)
target_link_libraries(maelstrom_app PRIVATE spdlog::spdlog)

# Install the application
install(TARGETS maelstrom_app
    RUNTIME DESTINATION bin
)
