cmake_minimum_required (VERSION 2.8.12)
project (snim C CXX)

# - Find MATH library
# Find the native MATH includes and library
# This module defines
#  MATH_INCLUDE_DIR, where to find hdf5.h, etc.
#  MATH_LIBRARIES, libraries to link against to use MATH.
#  MATH_FOUND, If false, do not try to use MATH.
# also defined, but not for general use are
#  MATH_LIBRARY, where to find the MATH library.

FIND_PATH(MATH_INCLUDE_DIR math.h)

SET(MATH_NAMES ${MATH_NAMES} m)
FIND_LIBRARY(MATH_LIBRARY NAMES ${MATH_NAMES} )

# handle the QUIETLY and REQUIRED arguments and set MATH_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MATH DEFAULT_MSG MATH_LIBRARY MATH_INCLUDE_DIR)

IF(MATH_FOUND)
  SET( MATH_LIBRARIES ${MATH_LIBRARY} )
ENDIF(MATH_FOUND)



option(LINK_STATIC_LIBS "Link with static libraries" OFF)

# C++ flags

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag(-std=c++11 HAVE_FLAG_CXX_11)
  if (HAVE_FLAG_CXX_11)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
  else ()
    message(FATAL_ERROR "Gorram! Your compiler is too frakking old. C++11 support is required.")
  endif ()
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -pedantic -fno-omit-frame-pointer -fno-inline-functions")
endif ()


include_directories(.)


set(SOURCES mainSnim.cpp
	snim.cpp 
)

if (LINK_STATIC_LIBS)
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	SET(BUILD_SHARED_LIBRARIES OFF)
	SET(CMAKE_EXE_LINKER_FLAGS "-static")
endif ()

add_executable(snim ${SOURCES})

target_link_libraries(snim ${MATH_LIBS})

