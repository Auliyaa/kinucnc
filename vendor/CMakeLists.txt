# nanosvg
add_library(nanosvg STATIC
  nanosvg-impl/nanosvg.cpp
)

target_include_directories(nanosvg PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/nanosvg/src
)

# tinyspline
include(ExternalProject)

ExternalProject_Add(tinyspline
  DOWNLOAD_COMMAND "" # disable download step

  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tinyspline
  BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/tinyspline.build
  INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/tinyspline.install

  CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release -DTINYSPLINE_BUILD_EXAMPLES=OFF -DTINYSPLINE_BUILD_TESTS=OFF -DTINYSPLINE_BUILD_DOCS=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/tinyspline.install
)

# avoid cmake complaining about the include directory not being available when creating the imported target below
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tinyspline.install/include")

add_library(tinyspline::static STATIC IMPORTED GLOBAL)

set_target_properties(tinyspline::static PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/tinyspline.install/lib64/libtinysplinecxx.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}/tinyspline.install/include"
  INTERFACE_LINK_LIBRARIES "\$<LINK_ONLY:m>"
)

add_dependencies(tinyspline::static tinyspline)

# ctpl
add_library(ctpl INTERFACE)
target_include_directories(ctpl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/CTPL)
