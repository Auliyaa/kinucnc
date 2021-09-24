add_executable(test-svg
  mylabel.cpp
  mylabel.h
  test.cpp)

target_link_libraries(test-svg
  Qt5::Widgets
  nanosvg
  tinyspline::static
)
