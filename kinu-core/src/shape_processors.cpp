#include <kinu/shape_processors.h>

#include <iostream> // FIXME: remove
#include <nanosvg.h>
#include <tinysplinecxx.h>

using namespace kinu::core;

std::tuple<double, double> centroid(path_t& path);

void shape_processors::default_processor(shape_t& out, NSVGshape* in, const shape_processor_params_t& params)
{
  size_t path_idx{0};

  auto path = in->paths;
  while (path != nullptr)
  {
    for (size_t ii = 0; ii < path->npts-1; ii += 3)
    {
      float* p = &path->pts[ii*2];

      tinyspline::BSpline spline(4,2,3);  // 4 control points, 2D, cubic spline
      spline.setControlPoints({
                                p[0],p[1],
                                p[2],p[3],
                                p[4],p[5],
                                p[6],p[7]
                              });
      // FIXME: this loop may stop prematurely
      for (float lstep=0; lstep<=1.0; lstep+=1.0/static_cast<float>(params.lsteps))
      {
        auto pt = spline.eval(lstep).result();
        out[path_idx].emplace_back(std::tuple<double,double>{pt[0],pt[1]});
      }
    }

    ++path_idx;
    path = path->next;
  }
}

void shape_processors::shift(double sx, double sy, shape_t& out, NSVGshape* in, const shape_processor_params_t& params)
{
  // start with the original shape using the default processor
  default_processor(out,in,params);
  // find shape's centroid
  double cx=0, cy=0;
  {
    double signed_area=0;
    for (const auto& path : out)
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

  // shift shape points according to their position relative to the centroid
  for (auto& path : out)
  {
    for (auto& [x,y] : path)
    {
      if (x>=cx) x+=sx;
      else if (x<cx) x-=sx;
      if (y>=cy) y+=sy;
      else if (y<cy) y-=sy;
    }
  }
}

