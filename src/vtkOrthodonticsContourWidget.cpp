/**
 * @file vtkOrthodonticsContourWidget.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @copyright copyright (c) 2022
 *
 * @copyright Copyright (c) 2022 Koninklijke Philips N.V. @ER2967
 *
 */

#include "vtkOrthodonticsContourWidget.hpp"

// vtk
#include <vtkObjectFactory.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>

vtkStandardNewMacro(vtkOrthodonticsContourWidget);

void vtkOrthodonticsContourWidget::Initialize(vtkProp* prop) {
  PolygonalSurfacePointPlacer->RemoveAllProps();
  PolygonalSurfacePointPlacer->AddProp(prop);
}

vtkOrthodonticsContourWidget::vtkOrthodonticsContourWidget()
    : PolygonalSurfacePointPlacer(PolygonalSurfacePointPlacerPtr::New()) {
  CreateDefaultRepresentation();
  auto* rep = GetOrientedGlyphContourRepresentation();
  rep->SetPointPlacer(PolygonalSurfacePointPlacer);
  vtkNew<vtkSphereSource> sphereSource;
  sphereSource->SetRadius(0.5);
  sphereSource->Update();
  vtkNew<vtkPolyData> cursor;
  cursor->ShallowCopy(sphereSource->GetOutput());
  rep->SetCursorShape(cursor);
  sphereSource->SetRadius(0.7);
  sphereSource->Update();
  vtkNew<vtkPolyData> activeCursor;
  activeCursor->ShallowCopy(sphereSource->GetOutput());
  rep->SetActiveCursorShape(activeCursor);
  rep->GetLinesProperty()->SetColor(0.0, 0.0, 1.0);
  rep->GetLinesProperty()->SetLineWidth(2.0);
}

vtkOrientedGlyphContourRepresentation*
vtkOrthodonticsContourWidget::GetOrientedGlyphContourRepresentation() {
  return reinterpret_cast<vtkOrientedGlyphContourRepresentation*>(
      this->WidgetRep);
}