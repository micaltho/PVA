cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

find_package(TBB REQUIRED)
list(APPEND CMAKE_PREFIX_PATH "/opt/intel/oneapie/tbb/latest")


add_executable(main_Dijkstra Dijkstra.cpp)

target_link_libraries(main_Dijkstra TBB::tbb)