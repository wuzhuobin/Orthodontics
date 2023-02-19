/**
 * @file QOrthodonticsFillHolesControllerWidget.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2023-02-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef Q_ORTHODONTICS_FILL_HOLES_CONTROLLER_WIDGET_HPP
#define Q_ORTHODONTICS_FILL_HOLES_CONTROLLER_WIDGET_HPP

#include "ui_QOrthodonticsFillHolesControllerWidget.h"

// qt
#include <QWidget>

class QOrthodonticsFillHolesController
    : public QWidget,
      public Ui::QOrthodonticsFillHolesControllerWidget {
  Q_OBJECT
 public:
  explicit QOrthodonticsFillHolesController(QWidget *parent = nullptr);
};

#endif  //! Q_ORTHODONTICS_FILL_HOLES_CONTROLLER_WIDGET_HPP