#include <kinu/bspline_processors.h>

using namespace kinu::core;

void bspline_processors::default_processor(path_t& out, const tinyspline::BSpline& spline, size_t lsteps)
{
  // FIXME: this can skip points. Ensure we have first and last
  for (float lstep=0; lstep<=1.0; lstep+=1.0/static_cast<float>(lsteps))
  {
    auto p = spline.eval(lstep).result();
    out.emplace_back(std::tuple<double,double>{p[0],p[1]});
  }
}
