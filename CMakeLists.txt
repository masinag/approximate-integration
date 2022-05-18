cmake_minimum_required(VERSION 3.14)
project(ApproximateIntegration CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

include(FetchContent)

set(default_build_type "Release")
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


message(STATUS "Fetching volesti from ${volesti_remote}")
FetchContent_Declare(volesti
    GIT_REPOSITORY ${volesti_remote}
    GIT_TAG 2e6572d9ba757de311f2a55b09abf8f9c02e932e
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
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_subdirectory(src)
