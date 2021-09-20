#pragma once

#include <vector>
#include <tuple>
#include <string>

#include <memory>

namespace kinu::svg
{

class svg_t
{
public:
  static constexpr const double DPI = 96.0;

  static svg_t from_file(const std::string& path);

  svg_t(const std::string& data, const std::string& id);
  svg_t(const svg_t&)=default;
  svg_t(svg_t&&)=default;
  virtual ~svg_t();

  std::vector<svg_t> subgroups() const;
  std::string id() const;

  std::vector<std::tuple<double,double>> segments() const;


private:
  std::string _data;
  std::string _id;
};

}
