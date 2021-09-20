#include <kinu/svg.h>

#include <fstream>
#include <cmath>
#include <ios>
#include <sstream>

#include <nanosvg.h>

#include <pugixml.hpp>

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
  if (!fd.is_open())
  {
    throw std::ios_base::failure{"failed to open for reading: " + path};
  }

  std::string contents((std::istreambuf_iterator<char>(fd)), std::istreambuf_iterator<char>());
  return std::move(svg_t{contents, path}); // TODO: extract filename
}

svg_t::svg_t(const std::string& data, const std::string& id)
  : _data{data}, _id{id}
{
}

svg_t::~svg_t()
{
}

std::vector<svg_t> svg_t::subgroups() const
{
  std::vector<svg_t> res;

  pugi::xml_document doc;
  doc.load_buffer(_data.c_str(), _data.size());
  for (const auto& group : doc.child("svg").children("g"))
  {
    pugi::xml_document tmp_svg;
    tmp_svg.append_child("svg");
    for (const auto& group_child : group.children())
    {
      tmp_svg.child("svg").append_copy(group_child);
    }
    std::stringstream tmp_str;
    tmp_svg.save(tmp_str);
    res.emplace_back(svg_t{tmp_str.str(), group.attribute("id").as_string()});
  }

  return res;
}

std::string svg_t::id() const
{
  return _id;
}

std::vector<std::tuple<double, double>> svg_t::segments() const
{
  // TODO: load image through nanosvg & flatten all beziers
  // TODO: multithreaded ?
  return {};
}
