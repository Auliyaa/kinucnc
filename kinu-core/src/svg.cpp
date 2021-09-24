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

#include <iostream>

void svg_t::default_processor(path_t& out, const tinyspline::BSpline& spline, size_t lsteps)
{
  // FIXME: this can skip points. Ensure we have first and last
  for (float lstep=0; lstep<=1.0; lstep+=1.0/static_cast<float>(lsteps))
  {
    auto p = spline.eval(lstep).result();
    out.emplace_back(std::tuple<double,double>{p[0],p[1]});
  }
}

bool svg_t::paths(std::vector<path_t>& out,
                  size_t lsteps,
                  bspline_processor_t processor,
                  size_t thread_count) const
{
  if (!out.empty()) return false;

  // parse svg data (use a copy since nanosvg modifies the actual string
  std::string data_cpy(_data.c_str(), _data.size());
  auto img = nsvgParse(const_cast<char*>(data_cpy.c_str()), "mm", DPI);

  // count the total number of paths to generate
  size_t path_cnt{0};
  {
    auto shape = img->shapes;
    while (shape != nullptr)
    {
      auto path = shape->paths;
      while (path != nullptr)
      {
        ++path_cnt;
        path = path->next;
      }
      shape = shape->next;
    }
  }

  // prepare the output storage for all workers
  out.resize(path_cnt);

  // thread pool
  ctpl::thread_pool pool;
  pool.resize(thread_count);

  // each worker flattens all bsplines from a svg path and pushes it into a the resulting structure
  auto worker = [&](int id, size_t out_idx, NSVGpath* path) -> void
  {
    for (size_t ii = 0; ii < path->npts-1; ii += 3)
    {
      float* p = &path->pts[ii*2];

      tinyspline::BSpline spline(4,2,3);  // 4 control points, 2D, cubic spline
      spline.setControlPoints({
                                p[0],p[1],
                                p[2],p[3],
                                p[4],p[5],
                                p[6],p[7]
                              });

      // forward to processor for any additional geometric transformation before flattening
      processor(out[out_idx],spline,lsteps);
    }
  };


  // recurse over all svg elements and spawn worker threads
  size_t out_idx{0};
  auto shape = img->shapes;
  while (shape != nullptr)
  {
    auto path = shape->paths;
    while (path != nullptr)
    {
      pool.push(std::bind(worker, std::placeholders::_1, out_idx, path));
      path = path->next;
      ++out_idx;
    }
    shape = shape->next;
  }
  pool.stop(true); // wait for workers to finish

  nsvgDelete(img);

  return true;
}
