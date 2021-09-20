#pragma once

#include <vector>
#include <list>
#include <tuple>
#include <string>

#include <memory>

#include <thread>

namespace kinu::svg
{

class svg_t
{
public:
  using shape_t = std::list<std::tuple<double,double>>;

  static constexpr const double DPI = 96.0;

  static svg_t from_file(const std::string& path);

  svg_t(const std::string& data, const std::string& id);
  svg_t(const svg_t&)=default;
  svg_t(svg_t&&)=default;
  virtual ~svg_t();

  std::vector<svg_t> subgroups() const;
  std::string id() const;

  std::vector<shape_t> segments(size_t lsteps=100, size_t thread_count=std::thread::hardware_concurrency()) const;

private:
  std::string _data;
  std::string _id;
};

}
