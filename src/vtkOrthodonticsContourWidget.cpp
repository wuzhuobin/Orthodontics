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

vtkStandardNewMacro(vtkOrthodonticsContourWidget);

void vtkOrthodonticsContourWidget::Initialize(vtkProp* prop) {
  PolygonalSurfacePointPlacer->RemoveAllProps();
  PolygonalSurfacePointPlacer->AddProp(prop);
}

vtkOrthodonticsContourWidget::vtkOrthodonticsContourWidget()
    : PolygonalSurfacePointPlacer(PolygonalSurfacePointPlacerPtr::New()) {
  CreateDefaultRepresentation();
  GetOrientedGlyphContourRepresentation()->SetPointPlacer(
      PolygonalSurfacePointPlacer);
}

vtkOrientedGlyphContourRepresentation*
vtkOrthodonticsContourWidget::GetOrientedGlyphContourRepresentation() {
  return reinterpret_cast<vtkOrientedGlyphContourRepresentation*>(
      this->WidgetRep);
}