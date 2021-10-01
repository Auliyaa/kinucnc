#include <kinu/geometry.h>

#include <stdlib.h>
#include <limits>

using namespace kinu::core;

point_t geometry::center(const shape_t& s)
{
  auto [xmin,ymin,xmax,ymax] = bouding_box(s);
  return {(xmax-xmin)/2,(ymax-ymin)/2};
}

point_t geometry::center(const bbox_t& bbox)
{
  const auto& [xmin,ymin,xmax,ymax] = bbox;
  return {(xmax-xmin)/2,(ymax-ymin)/2};
}

void geometry::scale(shape_t& s, double f)
{
  auto cref = s[0][0];

  // scale coordinates & get new centroid
  for (path_t& path : s)
  {
    for (auto& [x,y] : path)
    {
      x*=f;
      y*=f;
    }
  }
}

void geometry::translate(shape_t& s, const point_t& t)
{
  const auto& [tx,ty] = t;
  for (auto& path : s)
  {
    for (auto& [x,y] : path)
    {
      x+= tx; y+=ty;
    }
  }
}

point_t geometry::dist(const point_t& a, const point_t& b)
{
  return {std::get<0>(a)-std::get<0>(b), std::get<1>(a) - std::get<1>(b)};
}

point_t geometry::centroid(const shape_t& s)
{
  double cx=0, cy=0;
  {
    double signed_area=0;
    for (const auto& path : s)
    {
      for (size_t ii=0; ii < path.size(); ++ii)
      {
        auto& [x0,y0] = path[ii];
        auto& [x1,y1] = path[(ii+1)%path.size()];
        double a=(x0*y1) - (x1*y0);
        signed_area+=a;
        cx += (x0+x1)*a;
        cy += (y0+y1)*a;
      }
    }
    signed_area *= .5;
    cx /= (6*signed_area);
    cy /= (6*signed_area);
  }
  return {cx,cy};
}

bbox_t geometry::bouding_box(const shape_t& s)
{
  double xmin=std::numeric_limits<double>::max();
  double ymin=std::numeric_limits<double>::max();
  double xmax=std::numeric_limits<double>::min();
  double ymax=std::numeric_limits<double>::min();
  for (const auto& path : s)
  {
    for (const auto& [x,y] : path)
    {
      xmin = std::min<double>(xmin,x);
      xmax = std::max<double>(xmax,x);
      ymin = std::min<double>(ymin,y);
      ymax = std::max<double>(ymax,y);
    }
  }
  return {xmin,ymin,xmax,ymax};
}

double geometry::area(const bbox_t& b)
{
  const auto& [xmin,ymin,xmax,ymax] = b;
      return (xmax-xmin)*(ymax-ymin);
}
