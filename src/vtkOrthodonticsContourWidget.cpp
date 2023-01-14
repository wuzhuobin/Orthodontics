/**
 * @file vtkOrthodonticsContourWidget.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @copyright copyright (c) 2022
 *
 */

#include "vtkOrthodonticsContourWidget.hpp"

// vtk
#include <vtkCommand.h>
#include <vtkObjectFactory.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkPolygonalSurfaceContourLineInterpolator.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkWidgetEvent.h>

vtkStandardNewMacro(vtkOrthodonticsContourWidget);

void vtkOrthodonticsContourWidget::Initialize(vtkProp* prop) {
  PolygonalSurfacePointPlacer->RemoveAllProps();
  PolygonalSurfacePointPlacer->AddProp(prop);
}

vtkOrthodonticsContourWidget::vtkOrthodonticsContourWidget()
    : PolygonalSurfacePointPlacer(PolygonalSurfacePointPlacerPtr::New()) {
  CreateDefaultRepresentation();
  auto* rep = GetOrientedGlyphContourRepresentation();
  rep->SetLineInterpolator(
      vtkSmartPointer<vtkPolygonalSurfaceContourLineInterpolator>::New());
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

  CallbackMapper->SetCallbackMethod(
      vtkCommand::RightButtonReleaseEvent, vtkWidgetEvent::EndScale, this,
      vtkOrthodonticsContourWidget::EndSelectAction);
}

void vtkOrthodonticsContourWidget::EndSelectAction(vtkAbstractWidget* widget) {
  Superclass::EndSelectAction(widget);
  auto* self = reinterpret_cast<vtkOrthodonticsContourWidget*>(widget);
  auto* rep = self->GetOrientedGlyphContourRepresentation();
  if (rep->GetCurrentOperation() == vtkContourRepresentation::Inactive &&
      !rep->GetClosedLoop()) {
    self->CloseLoop();
  }
}

vtkOrientedGlyphContourRepresentation*
vtkOrthodonticsContourWidget::GetOrientedGlyphContourRepresentation() {
  return reinterpret_cast<vtkOrientedGlyphContourRepresentation*>(
      this->WidgetRep);
}