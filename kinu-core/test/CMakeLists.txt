add_executable(kinu-core-test
  kinu-core_test.cpp
  test_svg.cpp
)

target_link_libraries(kinu-core-test PRIVATE
  GTest::gtest
  kinu-svg
)

target_compile_definitions(kinu-core-test PRIVATE
  -DSVG_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/svg"
)

add_test(NAME test-kinu-core COMMAND kinu-core-test)


