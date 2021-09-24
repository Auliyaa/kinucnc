#include "svg_plotter.h"

#include <QtGui/QPainter>
#include <QtWidgets/QFileDialog>

#include <iostream>

#include "ui_svg_plotter.h"

svg_plotter::svg_plotter(QWidget* parent)
  : QWidget(parent),
    _ui{new Ui::svg_plotter}
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  _ui->setupUi(this);
}

svg_plotter::~svg_plotter()
{

}

void svg_plotter::paintEvent(QPaintEvent* event)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  QPainter p(this);
  p.translate(_ui->plot->pos());

  // FIXME: scaling is broken
  auto xscale = [this](double x) -> double
  {
    return static_cast<double>(x * std::min<int>(_ui->plot->width(),_ui->plot->height())) / static_cast<double>(_ui->spin_width->value());
  };
  auto yscale = [this](double x) -> double
  {
    return static_cast<double>(x * std::min<int>(_ui->plot->width(),_ui->plot->height())) / static_cast<double>(_ui->spin_height->value());
  };

  for (const auto& path : _paths)
  {
    auto [x1,y1] = path.front();
    for (const auto& [x2,y2] : path)
    {
      if (x1 != x2 || y1 != y2)
      {
        p.drawLine(xscale(x1), yscale(y1),
                   xscale(x2), yscale(y2));
      }
      x1 = x2;
      y1 = y2;
    }
  }
}

void svg_plotter::load_svg()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  auto svg_path = QFileDialog::getOpenFileName(this, "Open SVG", "", "Image files (*.svg)");
  _svg = kinu::core::svg_t::from_file(svg_path.toStdString());
  compute_paths();
}

void svg_plotter::compute_paths()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  _paths.clear();
  _svg.paths(_paths, _ui->lsteps_spin->value(), kinu::core::svg_t::default_processor, 1);
  repaint();
}

