cmake_minimum_required(VERSION 3.10)

project(Ecifircas)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# Set optimization and debugging flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native")

# Optional: Set specific flags for the target
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-g -O0>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-O2 -march=native>)

# Enable link time optimization (LTO)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

