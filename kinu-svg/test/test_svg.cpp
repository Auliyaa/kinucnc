#include <gtest/gtest.h>

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
