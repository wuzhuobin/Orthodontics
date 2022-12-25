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
#include <vtkClipClosedSurface.h>
#include <vtkFillHolesFilter.h>
#include <vtkPlaneCollection.h>
#include <vtkPolyData.h>
#include <vtkProp3D.h>
#include <vtkRenderWindow.h>

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
            enableInteractorObserver(mImplicitPlaneWidget2, enabled);
            mImplicitPlaneWidget2->Initialize(lowerPolyData);

            mViewWidget->renderWindow()->Render();
          });

  connect(
      mImplicitPlaneControllerWidget.pushButtonClip, &QPushButton::clicked,
      [this]() {
        auto* lowerProp3D = mViewWidget->getProp("Lower+AntagonistScan");
        lowerProp3D->SetVisibility(false);
        auto* plane = mImplicitPlaneWidget2->GetImplicitPlane();
        vtkNew<vtkPlaneCollection> planeCollection;
        planeCollection->AddItem(plane);
        auto* lowerPolyData =
            mViewWidget->getDataSet<vtkPolyData>("Lower+AntagonistScan");
        vtkNew<vtkClipClosedSurface> clipClosedSurface;
        clipClosedSurface->SetInputData(lowerPolyData);
        clipClosedSurface->SetClippingPlanes(planeCollection);
        clipClosedSurface->Update();
        vtkNew<vtkFillHolesFilter> fillHolesFilter;
        fillHolesFilter->SetInputConnection(clipClosedSurface->GetOutputPort());
        fillHolesFilter->SetHoleSize(100);
        fillHolesFilter->Update();
        mViewWidget->addPolyData("Lower+AntagonistScanClipped",
                                 fillHolesFilter->GetOutput());

        mViewWidget->renderWindow()->Render();
      });

  connect(mImplicitPlaneControllerWidget.pushButtonReset, &QPushButton::clicked,
          [this]() {
            auto* lowerProp3D = mViewWidget->getProp("Lower+AntagonistScan");
            lowerProp3D->SetVisibility(true);
            auto* lowerClippedProp3D =
                mViewWidget->getProp("Lower+AntagonistScanClipped");
            lowerClippedProp3D->SetVisibility(false);

            mViewWidget->renderWindow()->Render();
          });

  connect(mWidget->pushButtonSetupContour, &QPushButton::toggled,
          [this](auto enabled) {
            mContourControllerWidget.setVisible(enabled);
            enableInteractorObserver(mContourWidget, enabled);
            auto* lowerClippedProp3D =
                mViewWidget->getProp("Lower+AntagonistScanClipped");
            if (lowerClippedProp3D == nullptr) {
              return;
            }
            mContourWidget->Initialize(lowerClippedProp3D);
            mViewWidget->renderWindow()->Render();
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