cmake_minimum_required(VERSION 3.31)

project(udevpp
    VERSION 0.0.1
    DESCRIPTION "udevpp: c++ libudev wrapper for the poorly documented libudev"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES
    "src/*.cpp"
)

add_library(udevpp SHARED ${SOURCES})
target_link_libraries(udevpp drm udev)
target_include_directories(udevpp PUBLIC include)

target_compile_options(udevpp PRIVATE
    -Weverything
    -Wno-shadow
    -Wno-padded
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-unsafe-buffer-usage
    -Wno-pre-c++20-compat-pedantic
    -Wno-old-style-cast
)
