#include <iostream>

#define NANOSVG_IMPLEMENTATION
#include <nanosvg.h>
#undef NANOSVG_IMPLEMENTATION

#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtGui/QPainter>

#include "mylabel.h"

#include <tinysplinecxx.h>

const double DPI = 96;

template<typename T>
double to_mm(T px)
{
  return px * 25.4 / DPI;
}

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  auto img = nsvgParseFromFile(argv[1], "mm", DPI);
  if (img == nullptr)
  {
    std::cerr << "failed to load image" << std::endl;
    return 1;
  }

  MyLabel ml;
  std::cout << "width: " << to_mm(img->width) << " - height: " << to_mm(img->height) << std::endl;
  ml.resize(img->width+30, img->height+30);

  auto shape = img->shapes;
  while (shape != nullptr)
  {
    auto path = shape->paths;
    while (path != nullptr)
    {
      std::cout << "bezier:" << std::endl;;
      for (size_t ii = 0; ii < path->npts-1; ii += 3) {
        std::cout << "-- path:" << std::endl;;
        float* p = &path->pts[ii*2];
        std::cout << "-- [" << p[0] << "," << p[1] << "] ";
        std::cout << "-- [" << p[2] << "," << p[3] << "] ";
        std::cout << "-- [" << p[4] << "," << p[5] << "] ";
        std::cout << "-- [" << p[6] << "," << p[7] << "]" << std::endl;

        tinyspline::BSpline spline(4, // control points,
                                   2, // 2D,
                                   3 // cubic
                                   );
        std::vector<tinyspline::real> ctrlp = spline.controlPoints();
        ctrlp[0]  = p[0]; // x0
        ctrlp[1]  = p[1]; // y0
        ctrlp[2]  = p[2]; // x1
        ctrlp[3]  = p[3]; // y1
        ctrlp[4]  = p[4]; // x2
        ctrlp[5]  = p[5]; // y2
        ctrlp[6]  = p[6]; // x3
        ctrlp[7]  = p[7]; // y3
        spline.setControlPoints(ctrlp);

        std::vector<QPointF> shape;
        for (float e=0; e<=1.1; e+=0.1)
        {
          auto p = spline.eval(e).result();
          shape.emplace_back(QPointF(p[0], p[1]));
          std::cout << "-- -- " << e << ": " << p[0] << "," << p[1] << std::endl;
        }
        ml.add_shape(shape);
      }

      path = path->next;
    }

    shape = shape->next;
  }

  nsvgDelete(img);

  ml.show();

  return app.exec();
}
