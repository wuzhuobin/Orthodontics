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

  doubleSpinBoxLowerRange->setValue(0.0);
  doubleSpinBoxUpperRange->setValue(0.1);
  horizontalSliderLowerThreshold->setValue(
      doubleSpinBoxLowerThreshold->value());
  connect(doubleSpinBoxLowerRange,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          [this](auto min) {
            doubleSpinBoxLowerThreshold->setMinimum(min);
            horizontalSliderLowerThresholdUpdate();
          });
  connect(doubleSpinBoxUpperRange,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          [this](auto max) {
            doubleSpinBoxLowerThreshold->setMaximum(max);
            horizontalSliderLowerThresholdUpdate();
          });
  connect(doubleSpinBoxLowerThreshold,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          [this](auto value) { horizontalSliderLowerThresholdUpdate(); });
  connect(horizontalSliderLowerThreshold, &QSlider::valueChanged,
          [this](auto value) {
            auto max = doubleSpinBoxUpperRange->value();
            auto min = doubleSpinBoxLowerRange->value();
            auto v = value * 0.01 * (max - min) + min;
            doubleSpinBoxLowerThreshold->setValue(v);
          });
}

void QOrthodonticsContourControllerWidget::
    horizontalSliderLowerThresholdUpdate() {
  auto value = doubleSpinBoxLowerThreshold->value();
  auto min = doubleSpinBoxLowerRange->value();
  auto max = doubleSpinBoxUpperRange->value();
  auto v = 100 * (value - min) / (max - min) + 0.5;
  horizontalSliderLowerThreshold->setValue(v);
}