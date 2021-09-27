#pragma once

#include <functional>
#include <list>

#include <kinu/geometry.h>

struct NSVGshape;

namespace kinu::core
{

struct shape_processor_params_t
{
  size_t lsteps;
};

using shape_processor_t = std::function<void(shape_t& out, NSVGshape* in, const shape_processor_params_t& params)>;

struct shape_processors
{
  static void default_processor(shape_t& out, NSVGshape* in, const shape_processor_params_t& params);
  static void inside(double radius,shape_t& out, NSVGshape* in, const shape_processor_params_t& params);
};

}
