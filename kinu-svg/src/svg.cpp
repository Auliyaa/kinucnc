#include <kinu/svg.h>

#include <fstream>
#include <cmath>
#include <ios>
#include <sstream>

#include <nanosvg.h>

#include <pugixml.hpp>

#include <ctpl_stl.h>

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

void svg_t::default_processor(shape_t& out, const tinyspline::BSpline& spline, size_t lsteps)
{
  for (size_t lstep=0; lstep<=lsteps;++lstep)
  {
    auto p = spline.eval(double(lstep)/double(lsteps)).result();
    out.emplace_back(std::tuple<double,double>{p[0],p[1]});
  }
}

std::deque<svg_t::shape_t> svg_t::shapes(size_t lsteps, bspline_processor_t processor, size_t thread_count) const
{
  std::vector<std::deque<svg_t::shape_t>> buckets(thread_count);
  ctpl::thread_pool pool;
  pool.resize(thread_count);
  auto worker = [&](int id, NSVGpath* path) -> void
  {
    size_t bucket_off=buckets[id].size();
    buckets[id].resize(buckets[id].size() + path->npts-1);

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

      processor(buckets[id][bucket_off+ii],spline,lsteps);
    }
  };

  std::string data_cpy(_data.c_str(), _data.size());
  auto img = nsvgParse(const_cast<char*>(data_cpy.c_str()), "mm", DPI);

  auto shape = img->shapes;
  while (shape != nullptr)
  {
    auto path = shape->paths;
    while (path != nullptr)
    {
      pool.push(std::bind(worker, std::placeholders::_1, path));
      path = path->next;
    }
    shape = shape->next;
  }
  pool.stop(true); // wait for generation

  // merge buckets
  std::deque<svg_t::shape_t> result;
  for (const auto& bucket : buckets)
  {
    result.insert(result.end(), bucket.begin(), bucket.end());
  }

  nsvgDelete(img);

  return result;
}
