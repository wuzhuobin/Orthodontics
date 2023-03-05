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
#include <vtkArrowSource.h>
#include <vtkCallbackCommand.h>
#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkHandleWidget.h>
#include <vtkMapper.h>
#include <vtkNamedColors.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSeedRepresentation.h>
#include <vtkSphereHandleRepresentation.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkWidgetEvent.h>

vtkNew<vtkNamedColors> gColors;
vtkStandardNewMacro(vtkOrthodonticsFACCSeedWidget);

void vtkOrthodonticsFACCSeedWidget::SetEnabled(int enabled) {
  vtkAbstractWidget::SetEnabled(enabled);

  if (!enabled) {
    this->RequestCursorShape(VTK_CURSOR_DEFAULT);
    this->WidgetState = vtkSeedWidget::Start;
  }

  this->Render();
}

void vtkOrthodonticsFACCSeedWidget::CompleteInteraction() {
  auto seed0 = GetSeed(0);
  auto seed1 = GetSeed(1);
  if (seed0 == nullptr || seed1 == nullptr) {
    vtkErrorMacro(<< "At least 2 seeds are required.");
    return;
  }
  seed0->SetEnabled(false);
  seed1->SetEnabled(false);

  vtkNew<vtkArrowSource> arrowSource;
  arrowSource->Update();
  Facc->ShallowCopy(arrowSource->GetOutput());

  auto center = Tooth->GetCenter();

  auto point0 = seed0->GetHandleRepresentation()->GetWorldPosition();
  auto point1 = seed1->GetHandleRepresentation()->GetWorldPosition();

  std::array controlPoints = {point1[0] - point0[0], point1[1] - point0[1],
                              point1[2] - point0[2]};
  std::array arrow = {1.0, 0.0, 0.0};
  std::array<double, 3> axis;
  auto angle = vtkMath::DegreesFromRadians(
      vtkMath::AngleBetweenVectors(arrow.data(), controlPoints.data()));
  vtkMath::Cross(arrow.data(), controlPoints.data(), axis.data());

  vtkNew<vtkTransform> transform;
  transform->PostMultiply();
  transform->Scale(10, 10, 10);
  transform->RotateWXYZ(angle, axis.data());
  transform->Translate(center);

  vtkNew<vtkTransformPolyDataFilter> transformPolyDataFilter;
  transformPolyDataFilter->SetInputData(arrowSource->GetOutput());
  transformPolyDataFilter->SetTransform(transform);
  transformPolyDataFilter->Update();

  Facc->ShallowCopy(transformPolyDataFilter->GetOutput());

  Superclass::CompleteInteraction();
}

void vtkOrthodonticsFACCSeedWidget::RestartInteraction() {
  vtkNew<vtkPolyData> newFacc;
  Facc->ShallowCopy(newFacc);

  for (auto seed = GetSeed(0); seed != nullptr; seed = GetSeed(0)) {
    DeleteSeed(0);
  }

  Superclass::RestartInteraction();
}

void vtkOrthodonticsFACCSeedWidget::Initialize(vtkActor* actor,
                                               vtkPolyData* facc) {
  if (GetPointPlacer() == nullptr || actor == nullptr) {
    vtkWarningMacro(<< "Invalid point placer or invalid actor.");
    return;
  }
  GetPointPlacer()->RemoveAllProps();
  GetPointPlacer()->AddProp(actor);

  if (Tooth = vtkPolyData::SafeDownCast(actor->GetMapper()->GetInput());
      Tooth == nullptr) {
    vtkWarningMacro("Invalid tooth.");
    return;
  }

  Facc = facc;
  if (Facc == nullptr) {
    vtkWarningMacro(<< "Invalid FACC for output.");
    return;
  }
}

void vtkOrthodonticsFACCSeedWidget::AddPointAction(vtkAbstractWidget* widget) {
  auto self = reinterpret_cast<vtkOrthodonticsFACCSeedWidget*>(widget);
  auto rep = self->GetSeedRepresentation();

  // compute some info we need for all cases
  auto X = self->Interactor->GetEventPosition()[0];
  auto Y = self->Interactor->GetEventPosition()[1];

  // When a seed is placed, a new handle widget must be created and enabled.
  auto state = self->WidgetRep->ComputeInteractionState(X, Y);
  // Not allow to move seeds.
  if (state == vtkSeedRepresentation::NearSeed) {
    return;
  }

  // we are placing a new seed. Just make sure we aren't in a mode which
  // dictates we've placed all seeds.

  self->WidgetState = vtkSeedWidget::PlacingSeeds;
  double e[3];
  e[2] = 0.0;
  e[0] = static_cast<double>(X);
  e[1] = static_cast<double>(Y);

  // if the handle representation is constrained, check to see if
  // the position follows the constraint.
  if (!rep->GetHandleRepresentation()->CheckConstraint(
          self->GetCurrentRenderer(), e)) {
    return;
  }

  // Only add a seed when a cell is picked.
  if (!self->GetPointPlacer()->GetCellPicker()->Pick(
          e[0], e[1], e[2], self->GetCurrentRenderer())) {
    return;
  }

  int currentHandleNumber = rep->CreateHandle(e);
  vtkHandleWidget* currentHandle = self->CreateNewHandle();
  rep->SetSeedDisplayPosition(currentHandleNumber, e);
  currentHandle->SetEnabled(1);
  self->InvokeEvent(vtkCommand::PlacePointEvent, &(currentHandleNumber));
  self->InvokeEvent(vtkCommand::InteractionEvent, &(currentHandleNumber));

  self->EventCallbackCommand->SetAbortFlag(1);
  self->Render();

  // Complete interaction when there are 2 seeds.
  if (rep->GetNumberOfSeeds() >= GMaxNumberOfSeeds &&
      self->WidgetState == vtkSeedWidget::PlacingSeeds) {
    self->CompleteInteraction();
  }
}

vtkOrthodonticsFACCSeedWidget::vtkOrthodonticsFACCSeedWidget() {
  CallbackMapper->SetCallbackMethod(
      vtkCommand::LeftButtonPressEvent, vtkWidgetEvent::AddPoint, this,
      &vtkOrthodonticsFACCSeedWidget::AddPointAction);

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