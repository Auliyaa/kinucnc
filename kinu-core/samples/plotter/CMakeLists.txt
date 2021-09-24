add_executable(svg-plotter
  src/mainwindow.ui

  src/svg_plotter.h
  src/svg_plotter.cpp
  src/svg_plotter.ui

  src/plotter.cpp
)

target_include_directories(svg-plotter PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(svg-plotter PRIVATE
  kinu-svg
  Qt5::Widgets
)
