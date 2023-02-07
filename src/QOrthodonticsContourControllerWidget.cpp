/**
 * @file QOrthodonticsImplicitPlaneControllerWidget.cpp
 * @author wuzhuobin (zhoubin.wu@philips.com)
 * @brief
 * @version 0.1
 * @date 2022-12-25
 *
 * @copyright copyright (c) 2022
 *
 */

#include "QOrthodonticsContourControllerWidget.hpp"

// std
#include <iostream>

QOrthodonticsContourControllerWidget::QOrthodonticsContourControllerWidget(
    QWidget *parent) {
  setupUi(this);
}

void QOrthodonticsContourControllerWidget::
    horizontalSliderLowerThresholdUpdate() {
  auto value = doubleSpinBoxLowerThreshold->value();
  auto min = doubleSpinBoxLowerRange->value();
  auto max = doubleSpinBoxUpperRange->value();
  auto v = 100 * (value - min) / (max - min) + 0.5;
  horizontalSliderLowerThreshold->setValue(v);
}
void QOrthodonticsContourControllerWidget::
    on_doubleSpinBoxLowerRange_valueChanged(double value) {
  doubleSpinBoxLowerThreshold->setMinimum(value);
  horizontalSliderLowerThresholdUpdate();
}

void QOrthodonticsContourControllerWidget::
    on_doubleSpinBoxUpperRange_valueChanged(double value) {
  doubleSpinBoxLowerThreshold->setMaximum(value);
  horizontalSliderLowerThresholdUpdate();
}

void QOrthodonticsContourControllerWidget::
    on_doubleSpinBoxLowerThreshold_valueChanged(double value) {
  horizontalSliderLowerThresholdUpdate();
}

void QOrthodonticsContourControllerWidget::
    on_horizontalSliderLowerThreshold_valueChanged(int value) {
  auto max = doubleSpinBoxUpperRange->value();
  auto min = doubleSpinBoxLowerRange->value();
  auto v = value * 0.01 * (max - min) + min;
  doubleSpinBoxLowerThreshold->setValue(v);
}

void QOrthodonticsContourControllerWidget::on_checkBoxExtractRegions_toggled(
    bool checked) {
  spinBoxExtractRegions->setEnabled(checked);
}

void QOrthodonticsContourControllerWidget::
    on_spinBoxNumberOfRegions_valueChanged(int value) {
  spinBoxExtractRegions->setMaximum(value - 1);
}

void QOrthodonticsContourControllerWidget::on_pushButtonEnableAll_clicked(
    bool checked) {
  auto buttons = buttonGroupContour->buttons();
  for (auto button : buttons) {
    button->setChecked(true);
  }
}

void QOrthodonticsContourControllerWidget::on_pushButtonDisableAll_clicked(
    bool checked) {
  auto buttons = buttonGroupContour->buttons();
  for (auto button : buttons) {
    button->setChecked(false);
  }
}