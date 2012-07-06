## 
# Copyright (c) 2012 Roy van Dam <roy@8bit.cx>
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

# Define minimal required version of CMake.
cmake_minimum_required(VERSION 2.8.2)

# Project definition
project(FreeNES)
enable_language(CXX)

# Version information
set(FreeNES_VERSION_MAJOR "0")
set(FreeNES_VERSION_MINOR "1")
set(FreeNES_VERSION_PATCH "0")
set(FreeNES_VERSION "${FreeNES_VERSION_MAJOR}.${FreeNES_VERSION_MINOR}.${FreeNES_VERSION_PATCH}")
add_definitions(-D__BUILD_VERSION__="${FreeNES_VERSION}")

##
# CMake environment settings
#

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/resources/cmake")

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: none Debug Release."
        FORCE)
endif()

set(CMAKE_ERROR_FLAGS       "-Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-parameter -Wpointer-arith -Wcast-qual")

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} ${CMAKE_ERROR_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

##
# User configurable options
#

option(WITH_DEBUG   "Enable debug output."  ON)

if(WITH_DEBUG)
    add_definitions(-DWITH_DEBUG=)
endif()

##
# Project subdirectories
#

add_subdirectory(
    ${PROJECT_SOURCE_DIR}/source
    ${PROJECT_SOURCE_DIR}/output
)

##
# Add uninstall target
#

add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_MODULE_PATH}/uninstall.cmake"
)