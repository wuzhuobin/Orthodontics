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

#include "QSaveLoadUtil.hpp"

// vtk
#include <vtkActor.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderwindow.h>

// std
#include <iostream>

// qt
#include <QDebug>
#include <QFileInfo>

QOrthodonticsViewWidget::QOrthodonticsViewWidget(QWidget *parent)
    : QVTKOpenGLNativeWidget(parent) {
  vtkNew<vtkRenderer> renderer;
  renderWindow()->AddRenderer(renderer);
}

void QOrthodonticsViewWidget::addPolyData(const QString &path) {
  addPolyDataFromPath(path);
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
  vtkNew<vtkPolyData> data;
  auto success = QSaveLoadUtil::instance().loadPolyData(data, path);
  if (!success) {
    std::cerr << "Could not load " << path.toStdString() << '\n';
    return nullptr;
  }
  return renderPolyData(QFileInfo(path).baseName(), data);
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