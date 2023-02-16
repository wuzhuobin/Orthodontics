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
            auto* dataPolyData = mViewWidget.getDataSet<vtkPolyData>("Data");
            enableInteractorObserver(mImplicitPlaneWidget2, checked);
            mImplicitPlaneWidget2->Initialize(dataPolyData);

            mViewWidget.renderWindow()->Render();
          });

  connect(mImplicitPlaneControllerWidget.pushButtonClip, &QPushButton::clicked,
          [this]() {
            auto* dataProp3D = mViewWidget.getProp("Data");
            dataProp3D->SetVisibility(false);
            auto* plane = mImplicitPlaneWidget2->GetImplicitPlane();
            vtkNew<vtkPlaneCollection> planeCollection;
            planeCollection->AddItem(plane);
            auto* dataPolyData = mViewWidget.getDataSet<vtkPolyData>("Data");
            vtkNew<vtkClipClosedSurface> clipClosedSurface;
            clipClosedSurface->SetInputData(dataPolyData);
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
            mViewWidget.addPolyData("DataClipped",
                                    fillHolesFilter->GetOutput());

            mViewWidget.renderWindow()->Render();
          });
  connect(mImplicitPlaneControllerWidget.pushButtonReset, &QPushButton::clicked,
          [this]() {
            auto* dataProp3D = mViewWidget.getProp("Data");
            dataProp3D->SetVisibility(true);
            auto* dataClippedProp3D = mViewWidget.getProp("DataClipped");
            if (dataClippedProp3D != nullptr) {
              dataClippedProp3D->SetVisibility(false);
            }
          });
}

void QBridgeVtk::setupOrthodonticsContourControllerWidget() {
  connect(mImplicitPlaneControllerWidget.pushButtonReset, &QPushButton::clicked,
          [this]() {
            // auto* dataProp3D = mViewWidget.getProp("Data");
            // dataProp3D->SetVisibility(true);
            auto* dataClippedProp3D = mViewWidget.getProp("DataClipped");
            dataClippedProp3D->SetVisibility(true);
          });

  auto generateDraftContour = [this]() {
    auto* dataClipped = mViewWidget.getDataSet<vtkPolyData>("DataClipped");

    mGenerateContour->SetInputData(dataClipped);
    mGenerateContour->SetLowerThreshold(
        mContourControllerWidget.doubleSpinBoxLowerThreshold->value());
    if (mContourControllerWidget.checkBoxExtractRegions->isChecked()) {
      mGenerateContour->SetExtractedRegions(
          mContourControllerWidget.spinBoxExtractRegions->value());
    } else {
      mGenerateContour->SetExtractedRegions(-1);
    }
    mGenerateContour->Update();

    mViewWidget.addPolyData("DraftContours", mGenerateContour->GetOutput());

    auto* draftContoursActor = mViewWidget.getProp<vtkActor>("DraftContours");
    draftContoursActor->GetProperty()->SetRepresentationToWireframe();
    draftContoursActor->GetProperty()->SetColor(1, 0, 0);

    mViewWidget.renderWindow()->Render();
    mContourControllerWidget.spinBoxNumberOfRegions->setValue(
        mGenerateContour->GetNumberOfExtractedRegions());
  };

  connect(mWidget.pushButtonSetupContour, &QPushButton::toggled,
          [this, generateDraftContour](auto checked) {
            mContourControllerWidget.setVisible(checked);
            auto* dataProp3D = mViewWidget.getProp("Data");
            dataProp3D->SetVisibility(!checked);
            auto* dataClippedProp3D = mViewWidget.getProp("DataClipped");
            dataClippedProp3D->SetVisibility(checked);

            ///< @todo Disable this to speed up. The algorithm is not work.
            // if (checked) {
            //   generateDraftContour();
            // } else {
            //   if (auto draftContoursProp =
            //   mViewWidget.getProp("DraftContours");
            //       draftContoursProp != nullptr) {
            //     draftContoursProp->SetVisibility(false);
            //     mViewWidget.renderWindow()->Render();
            //   }
            // }
          });

  connect(mContourControllerWidget.doubleSpinBoxLowerThreshold,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          [generateDraftContour](auto /*value*/) { generateDraftContour(); });

  connect(mContourControllerWidget.spinBoxExtractRegions,
          QOverload<int>::of(&QSpinBox::valueChanged),
          [generateDraftContour](auto /*value*/) { generateDraftContour(); });

  connect(
      mContourControllerWidget.pushButtonContourExtraction,
      &QPushButton::clicked, [this]() {
        if (auto draftContoursProp = mViewWidget.getProp("DraftContours");
            draftContoursProp != nullptr) {
          draftContoursProp->SetVisibility(false);
        }
        auto* dataClippedPolyData =
            mViewWidget.getDataSet<vtkPolyData>("DataClipped");
        // 2-15, 18-31
        for (auto i = 0; i < GNumberOfTeeth; i++) {
          mContourExtractionFilters[i]->SetInputData(dataClippedPolyData);
          mContourExtractionFilters[i]->SetPointDataArrayName("PredictedID");
          mContourExtractionFilters[i]->SetLabel(i + 1);
          mContourExtractionFilters[i]->Update();
          mViewWidget.addPolyData("DataContour" + QString::number(i),
                                  mContourExtractionFilters[i]->GetOutput());
          auto* dataContourActor =
              mViewWidget.getProp<vtkActor>("DataContour" + QString::number(i));
          dataContourActor->GetProperty()->SetRepresentationToWireframe();
          dataContourActor->GetProperty()->SetColor(1, 0, 0);
        }
        mViewWidget.renderWindow()->Render();
      });

  for (auto side = 0; side < GNumberOfTeeth / 8; side++) {
    for (auto [i, b] = std::make_tuple(0 + side * 8, 1); i < (side + 1) * 8;
         i++, b++) {
      auto contourButton = mContourControllerWidget.findChild<QToolButton*>(
          "toolButtonContour" + QString::number(side + 1) + QString::number(b));
      connect(contourButton, &QToolButton::toggled, [this, i](auto checked) {
        auto& contourWidget = mContourWidgets[i];
        enableInteractorObserver(contourWidget, checked);
        if (!checked) {
          return;
        }
        if (contourWidget->GetWidgetState() != vtkContourWidget::Manipulate) {
          auto* dataClippedActor = mViewWidget.getProp<vtkActor>("DataClipped");
          auto* dataContour = mViewWidget.getDataSet<vtkPolyData>(
              "DataContour" + QString::number(i));
          contourWidget->Initialize(dataClippedActor, dataContour);
        }
        mViewWidget.renderWindow()->Render();
      });
    }
  }

  connect(mContourControllerWidget.pushButtonClip, &QPushButton::clicked,
          [this]() {
            auto* dataClippedProp3D = mViewWidget.getProp("DataClipped");
            dataClippedProp3D->SetVisibility(false);
            for (auto i = 0; i < GNumberOfTeeth; i++) {
              auto& contourWidget = mContourWidgets[i];
              auto rep = contourWidget->GetContourRepresentation();
              auto clipped = contourWidget->Clip();
              if (clipped != nullptr) {
                mViewWidget.addPolyData("Tooth" + QString::number(i), clipped);
              }
              enableInteractorObserver(contourWidget, false);
            }
            mViewWidget.renderWindow()->Render();
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