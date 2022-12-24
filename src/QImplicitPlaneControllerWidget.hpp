/**
 * @file QImplicitPlaneControllerWidget.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-12-24
 *
 * @copyright copyright (c) 2022
 *
 */

#ifndef Q_IMPLICIT_PLANE_WIDGET_HPP
#define Q_IMPLICIT_PLANE_WIDGET_HPP

#include "ui_QImplicitPlaneControllerWidget.h"

// qt
#include <QWidget>

class QImplicitPlaneControllerWidget
    : public QWidget,
      public Ui::QImplicitPlaneControllerWidget {
  Q_OBJECT
 public:
  explicit QImplicitPlaneControllerWidget(QWidget *parent = 0);
};

#endif  //! Q_IMPLICIT_PLANE_WIDGET_HPP