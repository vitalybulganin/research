cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME thread-pool)
project(${PROJECT_NAME})

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
# Reading version.
file(READ ${PROJECT_SOURCE_DIR}/version.txt PROJECT_VERSION)
# Building version include file.
configure_file(${PROJECT_SOURCE_DIR}/src/version.h.in ${PROJECT_SOURCE_DIR}/version.h)
# Collecting a list of sources.
file(GLOB_RECURSE INCLUDE_FILES *.h)
file(GLOB_RECURSE SOURCE_FILES *.cpp)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif(BUILD_TESTING)

add_library(${PROJECT_NAME} ${INCLUDE_FILES} ${SOURCE_FILES})

