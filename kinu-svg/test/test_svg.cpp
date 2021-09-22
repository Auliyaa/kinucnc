#include <gtest/gtest.h>

#include <chrono>
#include <kinu/svg.h>

#ifndef SVG_FOLDER
#define SVG_FOLDER ""
#endif // SVG_FOLDER

TEST(svg, from_file)
{
  EXPECT_NO_THROW(kinu::svg::svg_t::from_file(SVG_FOLDER"/square-10mm.svg"));
  EXPECT_NO_THROW(kinu::svg::svg_t::from_file(SVG_FOLDER"/groups.svg"));
}

TEST(svg, subgroups)
{
  {
    auto svg = kinu::svg::svg_t::from_file(SVG_FOLDER"/square-10mm.svg");
    auto subs = svg.subgroups();
    EXPECT_EQ(1, subs.size());
    EXPECT_STREQ("layer1", subs[0].id().c_str());
  }
  {
    auto svg = kinu::svg::svg_t::from_file(SVG_FOLDER"/groups.svg");
    auto svg_subs = svg.subgroups();
    EXPECT_EQ(1, svg_subs.size());
    EXPECT_STREQ("layer1", svg_subs[0].id().c_str());

    auto layer1_subs = svg_subs[0].subgroups();
    EXPECT_EQ(3, layer1_subs.size());
    EXPECT_STREQ("group.0", layer1_subs[0].id().c_str());
    EXPECT_STREQ("group.1", layer1_subs[1].id().c_str());
    EXPECT_STREQ("group.2", layer1_subs[2].id().c_str());

    auto group_0_subs = layer1_subs[0].subgroups();
    EXPECT_EQ(1, group_0_subs.size());
    EXPECT_STREQ("group.0.0", group_0_subs[0].id().c_str());

    auto group_0_0_subs = group_0_subs[0].subgroups();
    EXPECT_EQ(1, group_0_0_subs.size());
    EXPECT_STREQ("group.0.0.0", group_0_0_subs[0].id().c_str());
    EXPECT_EQ(0, group_0_0_subs[0].subgroups().size());

    auto group_1_subs = layer1_subs[1].subgroups();
    EXPECT_EQ(1, group_1_subs.size());
    EXPECT_STREQ("group.1.0", group_1_subs[0].id().c_str());
    EXPECT_EQ(0, group_1_subs[0].subgroups().size());

    auto group_2_subs = layer1_subs[2].subgroups();
    EXPECT_EQ(2, group_2_subs.size());
    EXPECT_STREQ("group.2.0", group_2_subs[0].id().c_str());
    EXPECT_STREQ("group.2.1", group_2_subs[1].id().c_str());
    EXPECT_EQ(0, group_2_subs[0].subgroups().size());
    EXPECT_EQ(0, group_2_subs[1].subgroups().size());
  }
}

TEST(svg, shapes)
{
  {
    auto svg = kinu::svg::svg_t::from_file(SVG_FOLDER"/square-10mm.svg");
    auto shapes = svg.shapes(1);
    EXPECT_EQ(4, shapes.size());
    shapes = svg.shapes(10);
    EXPECT_EQ(4, shapes.size());
    shapes = svg.shapes(100);
    EXPECT_EQ(4, shapes.size());
    shapes = svg.shapes(1000);
    EXPECT_EQ(4, shapes.size());
  }
  {
    auto svg = kinu::svg::svg_t::from_file(SVG_FOLDER"/groups.svg");
    auto shapes = svg.shapes(1);
    EXPECT_EQ(36, shapes.size());
    shapes = svg.shapes(10);
    EXPECT_EQ(36, shapes.size());
    shapes = svg.shapes(100);
    EXPECT_EQ(36, shapes.size());
    shapes = svg.shapes(1000);
    EXPECT_EQ(36, shapes.size());
  }
  {
    auto svg = kinu::svg::svg_t::from_file(SVG_FOLDER"/circle-10mm.svg");
    auto shapes = svg.shapes(1);
    EXPECT_EQ(5, shapes.size());
    for (const auto& shape : shapes)
    {
      EXPECT_EQ(2, shape.size());
    }
    shapes = svg.shapes(10);
    EXPECT_EQ(5, shapes.size());
    for (const auto& shape : shapes)
    {
      EXPECT_EQ(11, shape.size());
    }
    shapes = svg.shapes(100);
    EXPECT_EQ(5, shapes.size());
    for (const auto& shape : shapes)
    {
      EXPECT_EQ(101, shape.size());
    }
    shapes = svg.shapes(1000);
    EXPECT_EQ(5, shapes.size());
    for (const auto& shape : shapes)
    {
      EXPECT_EQ(1001, shape.size());
    }
  }
}

TEST(svg, stress)
{
  auto svg = kinu::svg::svg_t::from_file(SVG_FOLDER"/stress.svg");

  auto before = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
  auto shapes = svg.shapes(1000);
  auto after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
  auto took = after-before;

  size_t shape_cnt = shapes.size();
  size_t segment_cnt = 0;
  for (const auto& shape : shapes)
  {
    segment_cnt += shape.size();
  }

  std::cout << "took " << took << "ms to compute " << shape_cnt << " shapes & " << segment_cnt << " segments" << std::endl;
}
