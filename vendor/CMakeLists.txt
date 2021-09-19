# nanosvg
add_library(nanosvg STATIC
  nanosvg-impl/nanosvg.cpp
)

target_include_directories(nanosvg PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/nanosvg/src
)
