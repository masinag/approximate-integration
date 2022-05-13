cmake_minimum_required(VERSION 3.14)
project(ApproximateIntegration CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

include(FetchContent)
FetchContent_Declare(volesti
    GIT_REPOSITORY https://github.com/GeomScale/volesti
    GIT_TAG 2e6572d9ba757de311f2a55b09abf8f9c02e932e
)
FetchContent_MakeAvailable(volesti)

set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

add_definitions(-DDISABLE_NLP_ORACLES)

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

include_directories (BEFORE ${PROJECT_SOURCE_DIR}/include)
include_directories (BEFORE ${volesti_SOURCE_DIR}/external)
include_directories (BEFORE ${volesti_SOURCE_DIR}/external)
include_directories (BEFORE ${volesti_SOURCE_DIR}/external/minimum_ellipsoid)
include_directories (BEFORE ${volesti_SOURCE_DIR}/include)
include_directories (BEFORE ${volesti_SOURCE_DIR}/include/generators)
include_directories (BEFORE ${volesti_SOURCE_DIR}/include/integration)
include_directories (BEFORE ${volesti_SOURCE_DIR}/include/misc)
# add_subdirectory(src)
add_subdirectory(src)
