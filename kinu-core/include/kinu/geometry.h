#pragma once

#include <vector>
#include <tuple>

namespace kinu::core
{
using point_t = std::tuple<double,double>;
using path_t  = std::vector<std::tuple<double,double>>;
using shape_t = std::vector<path_t>;
using bbox_t  = std::tuple<double,double,double,double>;

struct geometry
{
  // returns the distance vector a-b
  static point_t dist(const point_t& a, const point_t& b);

  // returns the centroid of a polygon
  static point_t centroid(const shape_t&);
  // returns the center of a polygon (based on the bounding box)
  static point_t center(const shape_t&);
  // returns the center of a polygon (based on the bounding box)
  static point_t center(const bbox_t&);

  // returns the bouding box of a polygon
  static bbox_t bouding_box(const shape_t&);
  // returns the area of a given bouding box
  static double area(const bbox_t&);

  // scale a polygon
  static void scale(shape_t&, double);

  // translate a polygon
  static void translate(shape_t&,const point_t&);
};

} // kinu::core
