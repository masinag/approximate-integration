cmake_minimum_required(VERSION 3.14)
project(ApproximateIntegration CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

include(FetchContent)

set(default_build_type "Release")
set(exprtk_remote "https://github.com/ArashPartow/exprtk")
set(volesti_remote "https://github.com/GeomScale/volesti")
set(argparse_remote "https://github.com/p-ranav/argparse")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# for Eigen
add_compile_definitions("EIGEN_NO_DEBUG")

add_definitions(${CMAKE_CXX_FLAGS} "-std=c++17")  # enable C++11 standard
add_definitions(${CMAKE_CXX_FLAGS} "-O3")  # optimization of the compiler
#add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-lgsl")
add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-lm")
add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-ldl")
add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-DBOOST_NO_AUTO_PTR")
add_definitions(-DDISABLE_NLP_ORACLES)

message(STATUS "Fetching exprtk from ${exprtk_remote}")
FetchContent_Declare(exprtk
    GIT_REPOSITORY ${exprtk_remote}
    GIT_TAG 806c519c91fd08ba4fa19380dbf3f6e42de9e2d1
)
FetchContent_MakeAvailable(exprtk)

message(STATUS "Fetching volesti from ${volesti_remote}")
FetchContent_Declare(volesti
    GIT_REPOSITORY ${volesti_remote}
    GIT_TAG ba0e697a414bda101c8010676cc929131da2f064
)
FetchContent_MakeAvailable(volesti)
message(STATUS "Volesti fetched correctly")


message(STATUS "Fetching argparse from ${argparse_remote}")
FetchContent_Declare(argparse
    GIT_REPOSITORY ${argparse_remote}
)
FetchContent_MakeAvailable(argparse)
message(STATUS "Argparse fetched correctly")

include("${volesti_SOURCE_DIR}/external/cmake-files/Eigen.cmake")
GetEigen()

include("${volesti_SOURCE_DIR}/external/cmake-files/Boost.cmake")
GetBoost()

include("${volesti_SOURCE_DIR}/external/cmake-files/LPSolve.cmake")
GetLPSolve()

# Find lpsolve library
find_library(LP_SOLVE NAMES liblpsolve55.so PATHS /usr/lib/lp_solve)

if (NOT LP_SOLVE)
    message(FATAL_ERROR "This program requires the lp_solve library, and will not be compiled.")
endif()

message(STATUS "Library lp_solve found: ${LP_SOLVE}")

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# add_subdirectory(src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(${volesti_SOURCE_DIR}/external)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${volesti_SOURCE_DIR}/external)
include_directories(${volesti_SOURCE_DIR}/external/minimum_ellipsoid)
include_directories(${volesti_SOURCE_DIR}/include)
include_directories(${volesti_SOURCE_DIR}/include/generators)
include_directories(${volesti_SOURCE_DIR}/include/integration)
include_directories(${volesti_SOURCE_DIR}/include/misc)
include_directories(${exprtk_SOURCE_DIR})

add_subdirectory(apps)
