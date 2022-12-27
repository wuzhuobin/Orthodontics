/**
 * @file QSaveLoadUtil.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-12-27
 *
 * @copyright copyright (c) 2022
 *
 */

#include "QSaveLoadUtil.hpp"

// vtk
#include <vtkPolyData.h>

// qt
#include <QFileInfo>

const QSaveLoadUtil& QSaveLoadUtil::instance() {
  static QSaveLoadUtil instance;
  return instance;
}

bool QSaveLoadUtil::savePolyData(vtkPolyData* data, const QString& name) const {
  if (data == nullptr || name.isEmpty()) {
    return false;
  }
  switch (checkType(name)) {
    case Type::VTK:
      mPolyDataWriter->SetInputData(data);
      mPolyDataWriter->SetFileName(name.toLatin1());
      mPolyDataWriter->Update();
      return true;
    case Type::STL:
      mSTLWriter->SetInputData(data);
      mSTLWriter->SetFileName(name.toLatin1());
      mSTLWriter->Update();
      return true;
    default:
      return false;
  }
}

vtkPolyData* QSaveLoadUtil::loadPolyData(const QString& name) const {
  if (name.isEmpty()) {
    return nullptr;
  }
  switch (checkType(name)) {
    case Type::VTK:
      mPolyDataReader->SetFileName(name.toLatin1());
      mPolyDataReader->Update();
      return mPolyDataReader->GetOutput();
    case Type::STL:
      mSTLReader->SetFileName(name.toLatin1());
      mSTLReader->Update();
      return mSTLReader->GetOutput();
    default:
      return nullptr;
  }
}

QSaveLoadUtil::Type QSaveLoadUtil::checkType(const QString name) {
  auto ext = QFileInfo(name).suffix();
  if (ext.compare("stl", Qt::CaseInsensitive) == 0) {
    return Type::STL;
  }
  if (ext.compare("vtk", Qt::CaseInsensitive) == 0) {
    return Type::VTK;
  }
  return Type::Unknown;
}