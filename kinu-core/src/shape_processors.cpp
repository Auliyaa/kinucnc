#include <kinu/shape_processors.h>

#include <limits>
#include <iostream> // FIXME: remove

#include <nanosvg.h>
#include <tinysplinecxx.h>

using namespace kinu::core;

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
      for (float lstep=0; lstep<=1.0; lstep+=1.0/static_cast<float>(params.lsteps))
      {
        auto pt = spline.eval(lstep).result();
        out[path_idx].emplace_back(point_t{pt[0],pt[1]});
      }
    }

    ++path_idx;
    path = path->next;
  }
}

void shape_processors::inside(double radius, shape_t& out, NSVGshape* in, const shape_processor_params_t& params)
{
  // start by processing the shape with no shift
  default_processor(out,in,params);
  // see https://answers.opencv.org/question/67803/how-can-i-resize-an-image-from-the-center/
}
