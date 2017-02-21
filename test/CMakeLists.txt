cmake_minimum_required (VERSION 2.6)
project (testSnim C CXX)
find_package (Threads)

add_library(gtest ./gtest/gtest-all.cc)
include_directories(. ../.)


set(SOURCES run_all.cpp
	testSnim.cpp 
	../snim.cpp 
)

add_executable(testSnim ${SOURCES})

target_link_libraries(testSnim gtest ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBS})

