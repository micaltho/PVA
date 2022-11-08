find_package(TBB REQUIRED COMPONENTS tbb)

add_executable(main_sequentiell main_sequentiell.cpp)
add_executable(main_parallel main_parallel.cpp)

target_link_libraries(main_sequentiell tbb)
target_link_libraries(main_parallel tbb)