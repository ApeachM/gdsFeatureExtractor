cmake_minimum_required(VERSION 3.16)
project(gdsFeatureExtractor)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(gdsFileParser
        src/gdsCalmaRecords.h
        src/gdsFileParser.cpp
        src/gdsFileParser.h
        src/main.cpp)
