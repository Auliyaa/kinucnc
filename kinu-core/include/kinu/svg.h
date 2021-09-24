#pragma once

#include <vector>
#include <deque>
#include <list>
#include <tuple>
#include <string>
#include <memory>
#include <thread>
#include <functional>

#include <tinysplinecxx.h>

namespace kinu::core
{

class svg_t
{
public:
  using path_t = std::list<std::tuple<double,double>>;
  using bspline_processor_t = std::function<void(svg_t::path_t&,const tinyspline::BSpline&,size_t)>;

  static void default_processor(svg_t::path_t&,const tinyspline::BSpline&,size_t);

  static constexpr const double DPI = 96.0;

  static svg_t from_file(const std::string& path);

  svg_t(const std::string& data="<svg></svg>", const std::string& id="");
  svg_t(const svg_t&)=default;
  svg_t(svg_t&&)=default;
  svg_t& operator=(const svg_t&)=default;
  svg_t& operator=(svg_t&&)=default;
  virtual ~svg_t();

  std::vector<svg_t> subgroups() const;
  std::string id() const;

  bool paths(std::vector<path_t>& out,
             size_t lsteps=100,
             bspline_processor_t processor=&default_processor,
             size_t thread_count=std::thread::hardware_concurrency()) const;

private:
  std::string _data;
  std::string _id;
};

}

