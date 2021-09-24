#include "mylabel.h"

#include <QtGui/QPainter>

MyLabel::MyLabel()
  : QLabel()
{
}

void MyLabel::add_shape(const std::vector<QPointF> &s)
{
  _shapes.emplace_back(s);
}

void MyLabel::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  for (const auto& shape : _shapes)
  {
    QPointF p1 = shape[0];
    for (size_t ii=1; ii < shape.size(); ++ii)
    {
      QPointF p2 = shape[ii];
      painter.drawLine(p1,p2);
      p1=p2;
    }
  }
}
