# for Eigen
add_compile_definitions("EIGEN_NO_DEBUG")

add_definitions(${CMAKE_CXX_FLAGS} "-std=c++11")  # enable C++11 standard
add_definitions(${CMAKE_CXX_FLAGS} "-O3")  # optimization of the compiler
#add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-lgsl")
add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-lm")
add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-ldl")
add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-DBOOST_NO_AUTO_PTR")

add_executable (volesti_integrate_polynomial integrate_polynomial.cpp)

target_link_libraries(volesti_integrate_polynomial ${LP_SOLVE})
