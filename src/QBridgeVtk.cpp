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

// vtk
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkPolyData.h>

void QBridgeVtk::setViewWidget(QOrthodonticsViewWidget* viewWidget) {
  mViewWidget = viewWidget;
}

void QBridgeVtk::setWidget(QOrthodonticsWidget* widget) { mWidget = widget; }

void QBridgeVtk::setupConnection() {
  connect(mWidget->pushButtonSetupPlane, &QPushButton::toggled,
          [this](auto enabled) {
            mImplicitPlaneControllerWidget.setVisible(enabled);
            auto* lowerPolyData =
                mViewWidget->getDataSet<vtkPolyData>("Lower+AntagonistScan");
            mImplicitPlaneWidget2->CreateDefaultRepresentation();
            auto* rep = mImplicitPlaneWidget2->GetImplicitPlaneRepresentation();
            rep->SetPlaceFactor(1);
            rep->PlaceWidget(lowerPolyData->GetBounds());
            enableInteractorObserver(mImplicitPlaneWidget2, enabled);
          });
  connect(mWidget->pushButtonSetupContour, &QPushButton::toggled,
          [this](auto enabled) {
            enableInteractorObserver(mContourWidget, enabled);
          });
}

void QBridgeVtk::enableInteractorObserver(vtkInteractorObserver* observer,
                                          bool enabled) {
  if (observer == nullptr) {
    return;
  }
  observer->SetInteractor(enabled ? mViewWidget->interactor() : nullptr);
  observer->SetEnabled(enabled);
}