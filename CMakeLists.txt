cmake_minimum_required(VERSION 3.14)
project(ngram)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_PREFIX_PATH /usr/local/lib)

add_subdirectory(kenlm)
include_directories(kenlm)
include_directories(/usr/local/include)

find_library(FST fst REQUIRED)
find_library(FSTFAR fstfar REQUIRED)

message(STATUS ${FST})
message(STATUS ${FSTFAR})

add_executable(ngram main.cc)
target_link_libraries(ngram kenlm ${FST} ${FSTFAR})