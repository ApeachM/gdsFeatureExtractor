cmake_minimum_required(VERSION 3.16)
project(gdsFeatureExtractor)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(gdsFileParser
        src/GDS.cpp
        src/main.cpp
        src/DataStructures.cpp)

target_include_directories(gdsFileParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)