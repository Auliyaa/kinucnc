#pragma once

#include <QtGui/QPaintDevice>
#include <QtWidgets/QLabel>

#include <vector>

class MyLabel : public QLabel
{
  Q_OBJECT
public:
  MyLabel();
  void add_shape(const std::vector<QPointF>& s);
protected:
  virtual void paintEvent(QPaintEvent *event) override;
private:
  std::vector<std::vector<QPointF>> _shapes;
};

