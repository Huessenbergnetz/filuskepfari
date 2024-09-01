# SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

project(filuskepfari
    VERSION 0.0.1
    DESCRIPTION "Web service user bulk creator"
    HOMEPAGE_URL "https://github.com/Huessenbergnetz/filuskepfari"
    LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_VERSION_MAJOR 6)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network LinguistTools)

# Forbid in-tree building
if(${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
      message(STATUS "Please do an out-of-tree build:")
      message(STATUS "rm -f CMakeCache.txt && mkdir build && cd build; cmake .. && make")
      message(FATAL_ERROR "In-tree-build detected!")
endif(${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})

option(ENABLE_MAINTAINER_CFLAGS "Enable maintainer CFlags" OFF)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "filuskepfari default install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

include(GNUInstallDirs)

set(FSKEP_TRANS_DIR "${CMAKE_INSTALL_DATADIR}/filuskepfari" CACHE PATH "Directory to install translations insto")

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.20.0)
    GNUInstallDirs_get_absolute_install_dir(FSKEP_TRANS_FULL_DIR FSKEP_TRANS_DIR DATADIR)
else (CMAKE_VERSION VERSION_GREATER_EQUAL 3.20.0)
    GNUInstallDirs_get_absolute_install_dir(FSKEP_TRANS_FULL_DIR FSKEP_TRANS_DIR)
endif (CMAKE_VERSION VERSION_GREATER_EQUAL 3.20.0)

add_subdirectory(src)
add_subdirectory(translations)
