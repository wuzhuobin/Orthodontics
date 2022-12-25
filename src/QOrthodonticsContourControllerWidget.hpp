/**
 * @file QOrthodonticsContourControllerWidget.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-12-25
 *
 * @copyright copyright (c) 2022
 *
 */

#ifndef Q_ORTHODONTICS_CONTOUR_CONTROLLER_WIDGET_HPP
#define Q_ORTHODONTICS_CONTOUR_CONTROLLER_WIDGET_HPP

#include "ui_QOrthodonticsContourControllerWidget.h"

// qt
#include <QWidget>

class QOrthodonticsContourControllerWidget
    : public QWidget,
      public Ui::QOrthodonticsContourControllerWidget {
  Q_OBJECT
 public:
  QOrthodonticsContourControllerWidget(QWidget *parent = nullptr);
};

#endif  //! Q_ORTHODONTICS_CONTOUR_CONTROLLER_WIDGET_HPP