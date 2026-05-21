cmake_minimum_required(VERSION 3.10)
project(MiniBtop VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set(SOURCES
        src/main.cpp
        src/TerminalUI.cpp
        src/CPUReader.cpp
        src/MemoryReader.cpp
        src/ThermalReader.cpp
        src/ProcessReader.cpp
        src/SystemInfoReader.cpp
        src/NetworkReader.cpp
        src/MiniBtopApp.cpp
)

add_executable(minibtop ${SOURCES})
target_include_directories(minibtop PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)