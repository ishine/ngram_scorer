cmake_minimum_required(VERSION 3.14)
project(ngram)

set(CMAKE_CXX_STANDARD 11)
add_subdirectory(kenlm)
include_directories(kenlm)

add_executable(ngram main.cc)
target_link_libraries(kenlm)
target_compile_definitions(ngram PUBLIC KENLM_MAX_ORDER=5)