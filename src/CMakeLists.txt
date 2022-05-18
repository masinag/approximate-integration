# for Eigen
add_compile_definitions("EIGEN_NO_DEBUG")

add_definitions(${CMAKE_CXX_FLAGS} "-std=c++17")  # enable C++11 standard
add_definitions(${CMAKE_CXX_FLAGS} "-O3")  # optimization of the compiler
#add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-lgsl")
add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-lm")
add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-ldl")
add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-DBOOST_NO_AUTO_PTR")
add_definitions(-DDISABLE_NLP_ORACLES)

add_executable (volesti_integrate_polynomial integrate_polynomial.cpp)

target_include_directories (volesti_integrate_polynomial PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories (volesti_integrate_polynomial PUBLIC ${argparse_SOURCE_DIR}/include)
target_include_directories (volesti_integrate_polynomial PUBLIC ${volesti_SOURCE_DIR}/external)
target_include_directories (volesti_integrate_polynomial PUBLIC ${volesti_SOURCE_DIR}/external)
target_include_directories (volesti_integrate_polynomial PUBLIC ${volesti_SOURCE_DIR}/external/minimum_ellipsoid)
target_include_directories (volesti_integrate_polynomial PUBLIC ${volesti_SOURCE_DIR}/include)
target_include_directories (volesti_integrate_polynomial PUBLIC ${volesti_SOURCE_DIR}/include/generators)
target_include_directories (volesti_integrate_polynomial PUBLIC ${volesti_SOURCE_DIR}/include/integration)
target_include_directories (volesti_integrate_polynomial PUBLIC ${volesti_SOURCE_DIR}/include/misc)

target_link_libraries(volesti_integrate_polynomial ${LP_SOLVE})
