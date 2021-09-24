#pragma once

#include <vector>
#include <tuple>

namespace kinu::core
{
using path_t  = std::vector<std::tuple<double,double>>;
using shape_t = std::vector<path_t>;
} // kinu::core
