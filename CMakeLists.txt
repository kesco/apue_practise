cmake_minimum_required(VERSION 3.3)

project(apue_practise)

# 默认构建方式为Debug
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c99")

set(INSTALL_BIN_DIR "${PROJECT_BINARY_DIR}/bin")
set(INSTALL_LIB_DIR "${PROJECT_BINARY_DIR}/lib")

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES ${CMAKE_INSTALL_RPATH} isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
  set(CMAKE_INSTALL_RPATH "${INSTALL_LIB_DIR}")
endif("${isSystemDir}" STREQUAL "-1")

enable_testing()

# add_definitions(-DNO_DEBUG)

add_subdirectory(adk)
add_subdirectory(thread)
