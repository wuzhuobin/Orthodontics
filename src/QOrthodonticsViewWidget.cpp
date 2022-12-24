/**
 * @file QOrthodonticsViewWidget.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-12-21
 *
 * @copyright copyright (c) 2022
 *
 */

#include "QOrthodonticsViewWidget.hpp"

// vtk
#include <vtkActor.h>
// #include <vtkFillHolesFilter.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderwindow.h>
#include <vtkSTLReader.h>

// std
#include <iostream>

// qt
#include <QDebug>
#include <QFileInfo>

QOrthodonticsViewWidget::QOrthodonticsViewWidget(QWidget *parent)
    : QVTKOpenGLNativeWidget(parent) {
  vtkNew<vtkRenderer> renderer;
  renderWindow()->AddRenderer(renderer);

  const auto *lowerPath = "C:/ccode/Orthodontics/data/Lower+AntagonistScan.stl";
  auto *lowerActor = addPolyDataFromPath(lowerPath);
  // lowerActor->SetVisibility(false);
  // const auto *upperPath =
  //     "C:/ccode/Orthodontics/data/Upper+PreparationScan.stl";
  // auto *upperActor = addPolyDataFromPath(upperPath);
  // upperActor->SetVisibility(false);
  // const auto *lowerClippedPath =
  //     "C:/ccode/Orthodontics/data/Lower+AntagonistScanClipped.stl";

  // vtkNew<vtkSTLReader> stlReader;
  // stlReader->SetFileName(lowerClippedPath);
  // stlReader->Update();

  // vtkNew<vtkFillHolesFilter> fillHolesFilter;
  // fillHolesFilter->SetInputConnection(stlReader->GetOutputPort());
  // fillHolesFilter->SetHoleSize(100);
  // fillHolesFilter->Update();

  // renderPolyData(fillHolesFilter->GetOutput());
}

vtkDataSet *QOrthodonticsViewWidget::getDataSet(const QString &name) const {
  return std::get<0>(mDataBase[name]);
}

vtkAbstractMapper3D *QOrthodonticsViewWidget::getMapper(
    const QString &name) const {
  return std::get<1>(mDataBase[name]);
}

vtkProp3D *QOrthodonticsViewWidget::getProp(const QString &name) const {
  return std::get<2>(mDataBase[name]);
}

void QOrthodonticsViewWidget::addPolyData(const QString &name,
                                          vtkPolyData *polyData) {
  renderPolyData(name, polyData);
}

vtkActor *QOrthodonticsViewWidget::addPolyDataFromPath(const QString &path) {
  vtkNew<vtkSTLReader> stlReader;
  stlReader->SetFileName(path.toStdString().c_str());
  stlReader->Update();

  return renderPolyData(QFileInfo(path).baseName(), stlReader->GetOutput());
}

vtkActor *QOrthodonticsViewWidget::renderPolyData(const QString &name,
                                                  vtkPolyData *polyData) {
  auto DataTuple = mDataBase[name];
  auto oldProp = getProp(name);
  if (oldProp != nullptr) {
    renderWindow()->GetRenderers()->GetFirstRenderer()->RemoveViewProp(oldProp);
  }

  vtkNew<vtkPolyDataMapper> polyDataMapper;
  polyDataMapper->SetInputData(polyData);
  polyDataMapper->Update();

  vtkNew<vtkActor> actor;
  actor->SetMapper(polyDataMapper);
  mDataBase[name] = {polyData, polyDataMapper, actor};

  renderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);
  return actor;
}