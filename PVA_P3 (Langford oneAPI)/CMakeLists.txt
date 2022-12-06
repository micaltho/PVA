cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(TBB REQUIRED COMPONENTS tbb)
list(APPEND CMAKE_PREFIX_PATH "/opt/intel/oneapie/tbb/latest")

add_executable(main_Langford src/main.cpp)

target_link_libraries(main_Langford TBB::tbb)
target_link_libraries(main_Langford TBB::tbb)