cmake_minimum_required(VERSION 3.11)
project(qsim)


if(WIN32)
    set(CMAKE_CXX_FLAGS "/O2 /openmp")
else()
    set(CMAKE_CXX_FLAGS "-Ofast") 
endif()

if(APPLE)
    set(CMAKE_CXX_STANDARD 14)
    include_directories("/usr/local/include" "/usr/local/opt/llvm/include")
    link_directories("/usr/local/lib" "/usr/local/opt/llvm/lib")
endif()

INCLUDE(../GetPybind11.cmake)
pybind11_add_module(qsim_rvv pybind_main_rvv.cpp)

target_link_libraries(qsim_rvv PUBLIC OpenMP::OpenMP_CXX)
