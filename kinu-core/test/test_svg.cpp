#include <gtest/gtest.h>

#include <chrono>
#include <kinu/svg.h>

#ifndef SVG_FOLDER
#define SVG_FOLDER ""
#endif // SVG_FOLDER

TEST(svg, from_file)
{
  EXPECT_NO_THROW(kinu::core::svg_t::from_file(SVG_FOLDER"/square-10mm.svg"));
  EXPECT_NO_THROW(kinu::core::svg_t::from_file(SVG_FOLDER"/groups.svg"));
}

TEST(svg, subgroups)
{
  {
    auto svg = kinu::core::svg_t::from_file(SVG_FOLDER"/square-10mm.svg");
    auto subs = svg.subgroups();
    EXPECT_EQ(1, subs.size());
    EXPECT_STREQ("layer1", subs[0].id().c_str());
  }
  {
    auto svg = kinu::core::svg_t::from_file(SVG_FOLDER"/groups.svg");
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
    auto svg = kinu::core::svg_t::from_file(SVG_FOLDER"/square-10mm.svg");

    std::vector<kinu::core::svg_t::path_t> paths;
    EXPECT_TRUE(svg.paths(paths,1));
    EXPECT_EQ(1, paths.size());
    paths.clear();
    EXPECT_TRUE(svg.paths(paths,10));
    EXPECT_EQ(1, paths.size());
    paths.clear();
    EXPECT_TRUE(svg.paths(paths,100));
    EXPECT_EQ(1, paths.size());
    paths.clear();
    EXPECT_TRUE(svg.paths(paths,1000));
    EXPECT_EQ(1, paths.size());
  }
  {
    auto svg = kinu::core::svg_t::from_file(SVG_FOLDER"/groups.svg");
    std::vector<kinu::core::svg_t::path_t> paths;
    EXPECT_TRUE(svg.paths(paths,1));
    EXPECT_EQ(9, paths.size());
    paths.clear();
    EXPECT_TRUE(svg.paths(paths,10));
    EXPECT_EQ(9, paths.size());
    paths.clear();
    EXPECT_TRUE(svg.paths(paths,100));
    EXPECT_EQ(9, paths.size());
    paths.clear();
    EXPECT_TRUE(svg.paths(paths,1000));
    EXPECT_EQ(9, paths.size());
  }
  {
    auto svg = kinu::core::svg_t::from_file(SVG_FOLDER"/circle-10mm.svg");
    std::vector<kinu::core::svg_t::path_t> paths;
    EXPECT_TRUE(svg.paths(paths,1));
    EXPECT_EQ(1, paths.size());
    for (const auto& path : paths)
    {
      EXPECT_EQ(10, path.size());
    }
    paths.clear();
    EXPECT_TRUE(svg.paths(paths,10));
    EXPECT_EQ(1, paths.size());
    for (const auto& path : paths)
    {
      EXPECT_EQ(50, path.size());
    }
    paths.clear();
    EXPECT_TRUE(svg.paths(paths,100));
    EXPECT_EQ(1, paths.size());
    for (const auto& path : paths)
    {
      EXPECT_EQ(505, path.size());
    }
    paths.clear();
    EXPECT_TRUE(svg.paths(paths,1000));
    EXPECT_EQ(1, paths.size());
    for (const auto& path : paths)
    {
      EXPECT_EQ(5005, path.size());
    }
  }
}

TEST(svg, stress)
{
  auto svg = kinu::core::svg_t::from_file(SVG_FOLDER"/stress.svg");

  std::vector<kinu::core::svg_t::path_t> paths;
  auto before = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
  svg.paths(paths,1000);
  auto after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
  auto took = after-before;

  size_t path_cnt = paths.size();
  size_t segment_cnt = 0;
  for (const auto& path : paths)
  {
    segment_cnt += path.size();
  }

  std::cout << "took " << took << "ms to compute " << path_cnt << " paths & " << segment_cnt << " segments" << std::endl;
}