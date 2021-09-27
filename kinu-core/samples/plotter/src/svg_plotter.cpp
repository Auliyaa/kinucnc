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
  if (QApplication::arguments().size() >= 2)
  {
    _svg = kinu::core::svg_t::from_file(QApplication::arguments().at(1).toStdString());
    compute_paths();
  }
}

svg_plotter::~svg_plotter()
{

}

void svg_plotter::paintEvent(QPaintEvent* event)
{
  QPainter p(this);
  p.translate(_ui->plot->pos());

  auto px_dim = std::min<int>(_ui->plot->width(),_ui->plot->height());
  p.setPen(QPen(QColor(255,0,0), _ui->spin_tool_diam->value()));
  p.drawRect(0,0,px_dim,px_dim);

  auto px_scale = [this](double mm) -> double
  {
    return static_cast<double>(mm * std::min<int>(_ui->plot->width(),_ui->plot->height())) / std::max<double>(_ui->spin_height->value(), _ui->spin_width->value());
  };

  auto plot = [&](const decltype(_shapes_default)& shapes)
  {
    for (const auto& shape : shapes)
    {
      for (const auto& path : shape)
      {
        if (path.empty()) continue;
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
        auto[x2,y2] = path.front();
        p.drawLine(px_scale(x1), px_scale(y1),
                   px_scale(x2), px_scale(y2));
      }
    }
  };

  if (_ui->check_default->isChecked())
  {
    p.setPen(QPen(QColor(0,0,0), _ui->spin_tool_diam->value()));
    plot(_shapes_default);
  }

  if (_ui->check_inside->isChecked())
  {
    p.setPen(QPen(QColor(0,0,255), _ui->spin_tool_diam->value()));
    plot(_shapes_inside);
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

  if (_ui->check_default->isChecked())
  {
    _shapes_default.clear();
    _svg.shapes(_shapes_default, _ui->lsteps_spin->value(), kinu::core::shape_processors::default_processor);
  }

  if (_ui->check_inside->isChecked())
  {
    _shapes_inside.clear();

    kinu::core::shape_processor_t p = std::bind(&kinu::core::shape_processors::inside,
                                                static_cast<double>(_ui->spin_tool_diam->value()),
                                                std::placeholders::_1,
                                                std::placeholders::_2,
                                                std::placeholders::_3);

    _svg.shapes(_shapes_inside,
                _ui->lsteps_spin->value(),
                p,1);
  }

  repaint();
}

