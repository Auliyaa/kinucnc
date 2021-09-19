#include <kinu/svg.h>

#include <fstream>
#include <nanosvg.h>
#include <cmath>

using namespace kinu::svg;

// undo px conversion made by nanosvg, round to 1/1000
template<typename T>
double to_mm(T px)
{
  return std::round((px * 25.4 / svg_t::DPI)*1000.0)/1000.0;
}

svg_t svg_t::from_file(const std::string& path)
{
  std::ifstream fd(path, std::ios::in | std::ios::binary);
  std::string contents((std::istreambuf_iterator<char>(fd)), std::istreambuf_iterator<char>());
  return std::move(svg_t{contents.c_str()});
}

svg_t::svg_t(const char* buf)
  : _img{decltype(_img)(::nsvgParse(const_cast<char*>(buf), "mm", DPI), &::nsvgDelete)}
{
}

svg_t::svg_t(svg_t&& o)
  : _img{std::move(o._img)}
{
}

svg_t::~svg_t()
{
}

std::vector<svg_t> svg_t::groups() const
{
  // TODO
  return {};
}

std::vector<std::tuple<double, double>> svg_t::segments(double lfactor) const
{
  // TODO
  return {};
}

double svg_t::width() const
{
  return to_mm(_img->width);
}

double svg_t::height() const
{
  return to_mm(_img->height);
}
