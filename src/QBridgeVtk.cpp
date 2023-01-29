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

#include "QSaveLoadUtil.hpp"

// vtk
#include <vtkActor.h>
#include <vtkClipClosedSurface.h>
#include <vtkFillHolesFilter.h>
#include <vtkPlaneCollection.h>
#include <vtkPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>

// qt
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QRegularExpression>

QBridgeVtk::QBridgeVtk(QOrthodonticsViewWidget& viewWidget,
                       QOrthodonticsWidget& widget, QObject* parent)
    : QObject(parent), mViewWidget(viewWidget), mWidget(widget) {
  setupConnection();
}

void QBridgeVtk::setupConnection() {
  setupImplicitPlaneControllerWidget();
  setupOrthodonticsContourControllerWidget();

  connect(mWidget.pushButtonSave, &QPushButton::clicked, [this]() {
    QOrthodonticsWidget* parent = static_cast<QOrthodonticsWidget*>(sender());

    auto fileName =
        QFileDialog::getSaveFileName(parent, tr("Save Model"), QString(),
                                     tr("STL File(*.stl);;VTK File(*.vtk)"));

    if (fileName.isEmpty()) {
      return;
    }

    if (auto* data =
            mViewWidget.getDataSet<vtkPolyData>(QFileInfo(fileName).baseName());
        data == nullptr ||
        !QSaveLoadUtil::instance().savePolyData(data, fileName)) {
      QMessageBox::critical(parent, tr("Save Failed"),
                            tr("Cannot to save ") + fileName);
    }
  });
}

void QBridgeVtk::setupImplicitPlaneControllerWidget() {
  connect(mWidget.pushButtonSetupPlane, &QPushButton::toggled,
          [this](auto checked) {
            mImplicitPlaneControllerWidget.setVisible(checked);
            auto* lowerPolyData =
                mViewWidget.getDataSet<vtkPolyData>("Lower+AntagonistScan");
            enableInteractorObserver(mImplicitPlaneWidget2, checked);
            mImplicitPlaneWidget2->Initialize(lowerPolyData);

            mViewWidget.renderWindow()->Render();
          });

  connect(mImplicitPlaneControllerWidget.pushButtonClip, &QPushButton::clicked,
          [this]() {
            auto* lowerProp3D = mViewWidget.getProp("Lower+AntagonistScan");
            lowerProp3D->SetVisibility(false);
            auto* plane = mImplicitPlaneWidget2->GetImplicitPlane();
            vtkNew<vtkPlaneCollection> planeCollection;
            planeCollection->AddItem(plane);
            auto* lowerPolyData =
                mViewWidget.getDataSet<vtkPolyData>("Lower+AntagonistScan");
            vtkNew<vtkClipClosedSurface> clipClosedSurface;
            clipClosedSurface->SetInputData(lowerPolyData);
            clipClosedSurface->SetClippingPlanes(planeCollection);
            clipClosedSurface->Update();
            vtkNew<vtkPolyDataConnectivityFilter> polyDataConnectivityFilter;
            polyDataConnectivityFilter->SetInputConnection(
                clipClosedSurface->GetOutputPort());
            polyDataConnectivityFilter->SetExtractionMode(
                VTK_EXTRACT_LARGEST_REGION);
            polyDataConnectivityFilter->Update();
            vtkNew<vtkFillHolesFilter> fillHolesFilter;
            fillHolesFilter->SetInputConnection(
                polyDataConnectivityFilter->GetOutputPort());
            fillHolesFilter->SetHoleSize(100);
            fillHolesFilter->Update();
            mViewWidget.addPolyData("Lower+AntagonistScanClipped",
                                    fillHolesFilter->GetOutput());

            mViewWidget.renderWindow()->Render();
          });
}

void QBridgeVtk::setupOrthodonticsContourControllerWidget() {
  connect(mImplicitPlaneControllerWidget.pushButtonReset, &QPushButton::clicked,
          [this]() {
            // auto* lowerProp3D = mViewWidget.getProp("Lower+AntagonistScan");
            // lowerProp3D->SetVisibility(true);
            auto* lowerClippedProp3D =
                mViewWidget.getProp("Lower+AntagonistScanClipped");
            lowerClippedProp3D->SetVisibility(true);
          });

  auto generateDraftContour = [this]() {
    auto* lowerClippedCurvature = mViewWidget.getDataSet<vtkPolyData>(
        "Lower+AntagonistScanClippedCurvatures");

    mGenerateContour->SetInputData(lowerClippedCurvature);
    mGenerateContour->SetLowerThreshold(
        mContourControllerWidget.doubleSpinBoxLowerThreshold->value());
    if (mContourControllerWidget.checkBoxExtractRegions->isChecked()) {
      mGenerateContour->SetExtractedRegions(
          mContourControllerWidget.spinBoxExtractRegions->value());
    } else {
      mGenerateContour->SetExtractedRegions(-1);
    }
    mGenerateContour->Update();

    mViewWidget.addPolyData("DraftContour", mGenerateContour->GetOutput());

    auto* draftContourActor = mViewWidget.getProp<vtkActor>("DraftContour");
    draftContourActor->GetProperty()->SetRepresentationToWireframe();
    draftContourActor->GetProperty()->SetColor(1, 0, 0);

    mViewWidget.renderWindow()->Render();
    mContourControllerWidget.spinBoxNumberOfRegions->setValue(
        mGenerateContour->GetNumberOfExtractedRegions());
  };

  connect(mWidget.pushButtonSetupContour, &QPushButton::toggled,
          [this, generateDraftContour](auto checked) {
            mContourControllerWidget.setVisible(checked);
            auto* lowerProp3D = mViewWidget.getProp("Lower+AntagonistScan");
            lowerProp3D->SetVisibility(!checked);
            auto* lowerClippedProp3D =
                mViewWidget.getProp("Lower+AntagonistScanClipped");
            lowerClippedProp3D->SetVisibility(checked);

            if (checked) {
              generateDraftContour();
            } else {
              if (auto draftContourProp = mViewWidget.getProp("DraftContour");
                  draftContourProp != nullptr) {
                draftContourProp->SetVisibility(false);
                mViewWidget.renderWindow()->Render();
              }
            }
          });

  connect(mContourControllerWidget.doubleSpinBoxLowerThreshold,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          [generateDraftContour](auto /*value*/) { generateDraftContour(); });

  connect(mContourControllerWidget.spinBoxExtractRegions,
          QOverload<int>::of(&QSpinBox::valueChanged),
          [generateDraftContour](auto /*value*/) { generateDraftContour(); });

  auto contourButtons = mContourControllerWidget.findChildren<QToolButton*>(
      QRegularExpression("toolButtonContour[0-9]{2}"));
  for (const auto& contourButton : contourButtons) {
    auto contourWidget = vtkSmartPointer<vtkOrthodonticsContourWidget>::New();
    mContourWidgets << contourWidget;
    connect(contourButton, &QToolButton::toggled,
            [contourWidget, this](auto checked) {
              enableInteractorObserver(contourWidget, checked);
              if (!checked) {
                return;
              }
              auto* lowerClippedActor =
                  mViewWidget.getProp<vtkActor>("Lower+AntagonistScanClipped");
              contourWidget->Initialize(lowerClippedActor);
              mViewWidget.renderWindow()->Render();
            });
  }

  connect(mContourControllerWidget.pushButtonClip, &QPushButton::clicked,
          [this]() {
            auto* lowerClippedProp3D =
                mViewWidget.getProp("Lower+AntagonistScanClipped");
            lowerClippedProp3D->SetVisibility(false);
            for (auto contourWidget : mContourWidgets) {
              auto rep = contourWidget->GetContourRepresentation();
              auto clipped = contourWidget->Clip();
              if (clipped != nullptr) {
                mViewWidget.addPolyData("Teeth", clipped);
                mViewWidget.renderWindow()->Render();
              }
            }
          });
}

void QBridgeVtk::enableInteractorObserver(vtkInteractorObserver* observer,
                                          bool enabled) {
  if (observer == nullptr) {
    return;
  }
  observer->SetInteractor(enabled ? mViewWidget.interactor() : nullptr);
  observer->SetEnabled(enabled);
}