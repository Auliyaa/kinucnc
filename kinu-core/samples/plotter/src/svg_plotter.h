#pragma once

#include <kinu/svg.h>

#include <QtWidgets/QWidget>
#include <QtCore/QString>

namespace Ui
{
class svg_plotter;
}

class svg_plotter: public QWidget
{
  Q_OBJECT
public:
  svg_plotter(QWidget* parent=nullptr);
  virtual ~svg_plotter();

protected:
  virtual void paintEvent(QPaintEvent* event) override;

public slots:
  void load_svg();
  void compute_paths();

private:
  std::unique_ptr<Ui::svg_plotter> _ui;

  kinu::core::svg_t _svg;
  std::vector<kinu::core::path_t> _paths;
};
