/**
 * @file vtkOrthodonticsFACCSeedWidget.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2023-02-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "vtkOrthodonticsFACCSeedWidget.hpp"

// vtk
#include <vtkActor.h>
// #include <vtkMapper.h>
#include <vtkObjectFactory.h>
#include <vtkPointHandleRepresentation3D.h>
// #include <vtkPolyDataCollection.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkSeedRepresentation.h>

vtkStandardNewMacro(vtkOrthodonticsFACCSeedWidget);

void vtkOrthodonticsFACCSeedWidget::Initialize(vtkActor* actor) {
  if (GetPointPlacer() == nullptr || actor == nullptr) {
    vtkWarningMacro(<< "Invalid point placer or invalid actor.");
    return;
  }
  GetPointPlacer()->RemoveAllProps();
  GetPointPlacer()->AddProp(actor);

  //   auto* data = vtkPolyData::SafeDownCast(actor->GetMapper()->GetInput());
  //   if (data == nullptr) {
  //     vtkWarningMacro(<< "Invalid data.");
  //     return;
  //   }

  //   GetPointPlacer()->GetPolys()->AddItem(data);
}

vtkOrthodonticsFACCSeedWidget::vtkOrthodonticsFACCSeedWidget() {
  CreateDefaultRepresentation();
  vtkNew<vtkPolygonalSurfacePointPlacer> pointPlacer;
  vtkNew<vtkPointHandleRepresentation3D> handleRep;
  handleRep->SetPointPlacer(pointPlacer);
  GetSeedRepresentation()->SetHandleRepresentation(handleRep);
}

vtkPolygonalSurfacePointPlacer*
vtkOrthodonticsFACCSeedWidget::GetPointPlacer() {
  return static_cast<vtkPolygonalSurfacePointPlacer*>(
      GetSeedRepresentation()->GetHandleRepresentation()->GetPointPlacer());
}