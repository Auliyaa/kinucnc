#pragma once

#include <functional>

#include <kinu/geometry.h>

#include <tinysplinecxx.h>

namespace kinu::core
{

using bspline_processor_t = std::function<void(kinu::core::path_t&,const tinyspline::BSpline&,size_t)>;

struct bspline_processors
{
  static void default_processor(kinu::core::path_t&,const tinyspline::BSpline&,size_t);
};

}
