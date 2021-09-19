cmake_minimum_required(VERSION 3.21.0)

project(kinucnc VERSION 1.0.0 LANGUAGES CXX)

# setup toolchain
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# options
option(KINU_TESTS "enable unit tests compilation (requires gtest)" ON)

# qt5
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(KINU_TESTS)
  # gtest
  enable_testing()
  find_package(GTest REQUIRED)
endif(KINU_TESTS)

# external libraries
add_subdirectory(vendor)

# kinu-svg
add_subdirectory(kinu-svg)
