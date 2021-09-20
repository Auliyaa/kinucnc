# target: kinu-svg
add_library(kinu-svg STATIC
  src/svg.cpp
)

target_link_libraries(kinu-svg PUBLIC
  nanosvg
  pugixml::shared
)

target_include_directories(kinu-svg PUBLIC
  include
)

if(KINU_TESTS)
  add_subdirectory(test)
endif(KINU_TESTS)
