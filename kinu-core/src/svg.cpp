#include <kinu/svg.h>

#include <fstream>
#include <cmath>
#include <ios>
#include <sstream>

#include <nanosvg.h>

#include <pugixml.hpp>

#include <ctpl_stl.h>

using namespace kinu::core;

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
  return svg_t{contents, path}; // TODO: extract filename
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

bool svg_t::shapes(std::vector<shape_t>& out,
                   size_t lsteps,
                   shape_processor_t processor,
                   size_t thread_count) const
{
  if (!out.empty()) return false;

  // parse svg data (use a copy since nanosvg modifies the actual string
  std::string data_cpy(_data.c_str(), _data.size());
  auto img = nsvgParse(const_cast<char*>(data_cpy.c_str()), "mm", DPI);

  // pre-allocate everything
  {
    size_t shape_cnt{0};
    auto shape = img->shapes;
    while (shape != nullptr)
    {
      ++shape_cnt;
      shape = shape->next;
    }
    out.resize(shape_cnt);
  }
  {
    size_t shape_idx{0};
    auto shape = img->shapes;
    while (shape != nullptr)
    {
      size_t path_cnt{0};
      auto path = shape->paths;
      while (path != nullptr)
      {
        ++path_cnt;
        path = path->next;
      }
      out[shape_idx].resize(path_cnt);

      ++shape_idx;
      shape = shape->next;
    }
  }

  // thread pool
  ctpl::thread_pool pool;
  pool.resize(thread_count);

  shape_processor_params_t params{lsteps};

  // each worker flattens all bsplines from a svg shape and pushes it into a the resulting structure
  auto worker = [&](int id, size_t out_idx, NSVGshape* shape) -> void
  {
    // forward to processor for any additional geometric transformation & flattening
    processor(out[out_idx],shape,params);
  };

  // recurse over all svg elements and spawn worker threads
  size_t out_idx{0};
  auto shape = img->shapes;
  while (shape != nullptr)
  {
    pool.push(std::bind(worker, std::placeholders::_1, out_idx, shape));
    ++out_idx;
    shape = shape->next;
  }
  pool.stop(true); // wait for workers to finish

  nsvgDelete(img);

  return true;
}
