# SPDX-License-Identifier: Apache-2.0

##########################################################################
# Project Setup                                                          #
##########################################################################
cmake_minimum_required(VERSION 3.13.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(lorawanz)

##########################################################################
# Static Analysis                                                        #
##########################################################################
find_program(CPPCHECK NAMES cppcheck)
if(CPPCHECK)
  message(STATUS "Found cppcheck: ${CPPCHECK}")
  set(CMAKE_C_CPPCHECK ${CPPCHECK}
    --enable=all
    --inline-suppr
    --quiet
    --std=c${CMAKE_C_STANDARD}
    --suppress=missingInclude
    --suppress=unusedFunction
    --template=gcc
    )
endif()

##########################################################################
# Local                                                                  #
##########################################################################
add_subdirectory(src)
