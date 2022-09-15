add_executable (volesti_integrate volesti_integrate.cpp)

target_include_directories (volesti_integrate PUBLIC ${argparse_SOURCE_DIR}/include)

target_link_libraries(volesti_integrate ${LP_SOLVE})