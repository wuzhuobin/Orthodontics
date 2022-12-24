/**
 * @file QBridgeVtk.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-12-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "QBridgeVtk.hpp"

#include "QOrthodonticsViewWidget.hpp"
#include "QOrthodonticsWidget.hpp"

// vtk
#include <vtkContourWidget.h>

void QBridgeVtk::setViewWidget(QOrthodonticsViewWidget* viewWidget) {
  mViewWidget = viewWidget;
}

void QBridgeVtk::setWidget(QOrthodonticsWidget* widget) { mWidget = widget; }

void QBridgeVtk::setupConnection() {
  connect(mWidget->pushButtonSetupContour, &QPushButton::clicked,
          [this](auto enabled) {
            static vtkNew<vtkContourWidget> contourWidget;
            contourWidget->SetInteractor(mViewWidget->interactor());
            contourWidget->SetEnabled(enabled);
          });
}