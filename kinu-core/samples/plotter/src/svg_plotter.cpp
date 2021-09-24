#include "svg_plotter.h"

#include <QtGui/QPainter>
#include <QtWidgets/QFileDialog>

#include <iostream>

#include "ui_svg_plotter.h"

svg_plotter::svg_plotter(QWidget* parent)
  : QWidget(parent),
    _ui{new Ui::svg_plotter}
{
  _ui->setupUi(this);
}

svg_plotter::~svg_plotter()
{

}

void svg_plotter::paintEvent(QPaintEvent* event)
{
  QPainter p(this);
  p.translate(_ui->plot->pos());

  auto px_dim = std::min<int>(_ui->plot->width(),_ui->plot->height());
  p.setPen(QColor(255,0,0));
  p.drawRect(0,0,px_dim,px_dim);
  p.setPen(QColor(0,0,0));

  auto px_scale = [this](double mm) -> double
  {
    return static_cast<double>(mm * std::min<int>(_ui->plot->width(),_ui->plot->height())) / std::max<double>(_ui->spin_height->value(), _ui->spin_width->value());
  };

  for (const auto& path : _paths)
  {
    auto [x1,y1] = path.front();
    for (const auto& [x2,y2] : path)
    {
      if (x1 != x2 || y1 != y2)
      {
        p.drawLine(px_scale(x1), px_scale(y1),
                   px_scale(x2), px_scale(y2));
      }
      x1 = x2;
      y1 = y2;
    }
  }
}

void svg_plotter::load_svg()
{
  auto svg_path = QFileDialog::getOpenFileName(this, "Open SVG", "", "Image files (*.svg)");
  _svg = kinu::core::svg_t::from_file(svg_path.toStdString());
  compute_paths();
}

void svg_plotter::compute_paths()
{
  _paths.clear();
  _svg.paths(_paths, _ui->lsteps_spin->value(), kinu::core::svg_t::default_processor, 1);
  repaint();
}

