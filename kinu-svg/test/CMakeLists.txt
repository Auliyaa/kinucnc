add_executable(kinu-svg-test
  kinu-svg_test.cpp
  test_svg.cpp
)

target_link_libraries(kinu-svg-test PRIVATE
  GTest::gtest
  kinu-svg
)

target_compile_definitions(kinu-svg-test PRIVATE
  -DSVG_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/svg"
)

add_test(NAME test-kinu-svg COMMAND kinu-svg-test)


