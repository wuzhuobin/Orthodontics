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
#include <vtkNamedColors.h>
#include <vtkObjectFactory.h>
// #include <vtkPointHandleRepresentation3D.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkProperty.h>
#include <vtkSeedRepresentation.h>
#include <vtkSphereHandleRepresentation.h>

vtkNew<vtkNamedColors> gColors;
vtkStandardNewMacro(vtkOrthodonticsFACCSeedWidget);

void vtkOrthodonticsFACCSeedWidget::Initialize(vtkActor* actor) {
  if (GetPointPlacer() == nullptr || actor == nullptr) {
    vtkWarningMacro(<< "Invalid point placer or invalid actor.");
    return;
  }
  GetPointPlacer()->RemoveAllProps();
  GetPointPlacer()->AddProp(actor);
}

vtkOrthodonticsFACCSeedWidget::vtkOrthodonticsFACCSeedWidget() {
  CreateDefaultRepresentation();
  vtkNew<vtkPolygonalSurfacePointPlacer> pointPlacer;
  vtkNew<vtkSphereHandleRepresentation> handleRep;
  handleRep->GetProperty()->SetColor(gColors->GetColor3d("red").GetData());
  handleRep->SetPointPlacer(pointPlacer);
  GetSeedRepresentation()->SetHandleRepresentation(handleRep);
}

vtkPolygonalSurfacePointPlacer*
vtkOrthodonticsFACCSeedWidget::GetPointPlacer() {
  return static_cast<vtkPolygonalSurfacePointPlacer*>(
      GetSeedRepresentation()->GetHandleRepresentation()->GetPointPlacer());
}