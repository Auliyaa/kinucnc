# target: kinu-svg
add_library(kinu-svg STATIC
  src/svg.cpp
  src/bspline_processors.cpp
)

target_link_libraries(kinu-svg PUBLIC
  nanosvg
  pugixml::shared
  tinyspline::static
  Threads::Threads
  ctpl
)

target_include_directories(kinu-svg PUBLIC
  include
)

if(KINU_TESTS)
  add_subdirectory(test)
endif(KINU_TESTS)

if(KINU_SAMPLES)
  add_subdirectory(samples)
endif(KINU_SAMPLES)
