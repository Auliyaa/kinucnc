#include <gtest/gtest.h>

#include <kinu/svg.h>

#ifndef SVG_FOLDER
#define SVG_FOLDER ""
#endif // SVG_FOLDER

TEST(svg, from_file)
{
  kinu::svg::svg_t svg = kinu::svg::svg_t::from_file(SVG_FOLDER"/square-10mm.svg");

  EXPECT_DOUBLE_EQ(svg.width(),10.);
  EXPECT_DOUBLE_EQ(svg.height(),10.);
}
