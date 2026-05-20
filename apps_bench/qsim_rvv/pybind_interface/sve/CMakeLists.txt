cmake_minimum_required(VERSION 3.11)
project(qsim)


if(WIN32)
    set(CMAKE_CXX_FLAGS "/O2 /openmp")
else()
    set(CMAKE_CXX_FLAGS "-Ofast  -march=armv8.5-a+sve -mcpu=neoverse-v2 -fopt-info-vec=vectorization.log") 
endif()

if(APPLE)
    set(CMAKE_CXX_STANDARD 14)
    include_directories("/usr/local/include" "/usr/local/opt/llvm/include")
    link_directories("/usr/local/lib" "/usr/local/opt/llvm/lib")
endif()

INCLUDE(../GetPybind11.cmake)
pybind11_add_module(qsim_sve pybind_main_sve.cpp)

target_link_libraries(qsim_sve PUBLIC OpenMP::OpenMP_CXX)
