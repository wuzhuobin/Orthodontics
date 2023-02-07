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

 private:
  void horizontalSliderLowerThresholdUpdate();
  Q_SLOT void on_doubleSpinBoxLowerRange_valueChanged(double value);
  Q_SLOT void on_doubleSpinBoxUpperRange_valueChanged(double value);
  Q_SLOT void on_doubleSpinBoxLowerThreshold_valueChanged(double value);
  Q_SLOT void on_horizontalSliderLowerThreshold_valueChanged(int value);
  Q_SLOT void on_checkBoxExtractRegions_toggled(bool checked);
  Q_SLOT void on_spinBoxNumberOfRegions_valueChanged(int value);
  Q_SLOT void on_pushButtonEnableAll_clicked(bool checked = false);
  Q_SLOT void on_pushButtonDisableAll_clicked(bool checked = false);
};

#endif  //! Q_ORTHODONTICS_CONTOUR_CONTROLLER_WIDGET_HPP