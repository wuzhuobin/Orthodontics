/**
 * @file QOrthodonticsGingivalLineGenerateControllerWidget.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2023-02-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef Q_ORTHODONTICS_GINGIVAL_LINE_GENERATE_CONTROLLER_WIDGET_HPP
#define Q_ORTHODONTICS_GINGIVAL_LINE_GENERATE_CONTROLLER_WIDGET_HPP

#include "ui_QOrthodonticsGingivalLineGenerateControllerWidget.h"

// qt
#include <QWidget>

class QOrthodonticsGingivalLineGenerateControllerWidget
    : public QWidget,
      public Ui::QOrthodonticsGingivalLineGenerateControllerWidget {
  Q_OBJECT
 public:
  explicit QOrthodonticsGingivalLineGenerateControllerWidget(
      QWidget *parent = nullptr);
};

#endif  //! Q_ORTHODONTICS_GINGIVAL_LINE_GENERATE_CONTROLLER_WIDGET_HPP