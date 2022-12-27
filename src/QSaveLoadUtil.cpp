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

const QSaveLoadUtil& QSaveLoadUtil::instance() {
  static QSaveLoadUtil instance;
  return instance;
}

bool QSaveLoadUtil::savePolyData(vtkPolyData* data, const QString& name) const {
  if (data == nullptr || name.isEmpty()) {
    return false;
  }
  mPolyDataWriter->SetInputData(data);
  mPolyDataWriter->SetFileName(name.toLatin1());
  mPolyDataWriter->Update();
  return true;
}

vtkPolyData* QSaveLoadUtil::loadPolyData(const QString& name) const {
  if (name.isEmpty()) {
    return nullptr;
  }

  mPolyDataReader->SetFileName(name.toLatin1());
  mPolyDataReader->Update();
  return mPolyDataReader->GetOutput();
}