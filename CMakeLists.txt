cmake_minimum_required(VERSION 3.21.0)

project(kinucnc VERSION 1.0.0 LANGUAGES CXX)

# setup toolchain
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# options
option(KINU_TESTS "enable unit tests compilation (requires gtest)" ON)
option(KINU_SAMPLES "enable samples compilation" ON)

# threads
find_package(Threads REQUIRED)

# qt5
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# pugixml
find_package(pugixml REQUIRED)

# gtest
if(KINU_TESTS)
  enable_testing()
  find_package(GTest REQUIRED)
endif(KINU_TESTS)

# builtin external libraries
add_subdirectory(vendor)

# kinu-svg
add_subdirectory(kinu-core)
