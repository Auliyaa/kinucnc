#pragma once

#include <vector>
#include <tuple>
#include <string>

#include <memory>

struct NSVGimage;

namespace kinu::svg
{

class svg_t
{
public:
  static constexpr const double DPI = 96.0;

  static svg_t from_file(const std::string& path);

  svg_t(const char* buf);
  svg_t(const svg_t&)=delete;
  svg_t(svg_t&&);
  virtual ~svg_t();

  std::vector<svg_t> groups() const;
  std::vector<std::tuple<double,double>> segments(double lfactor) const;

  double width() const;
  double height() const;

private:
  std::unique_ptr<NSVGimage, void(*)(NSVGimage*)> _img;
};

}
