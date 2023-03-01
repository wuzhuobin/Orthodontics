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
#include <vtkCallbackCommand.h>
#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkHandleWidget.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSeedRepresentation.h>
#include <vtkSphereHandleRepresentation.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkWidgetEvent.h>

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

void vtkOrthodonticsFACCSeedWidget::AddPointAction(vtkAbstractWidget* widget) {
  // Superclass::AddPointAction(widget);
  auto self = reinterpret_cast<vtkOrthodonticsFACCSeedWidget*>(widget);
  auto rep = self->GetSeedRepresentation();

  // compute some info we need for all cases
  auto X = self->Interactor->GetEventPosition()[0];
  auto Y = self->Interactor->GetEventPosition()[1];

  // When a seed is placed, a new handle widget must be created and enabled.
  auto state = self->WidgetRep->ComputeInteractionState(X, Y);
  if (self->WidgetState == vtkSeedWidget::PlacedSeeds) {
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
  // if the picker pick nothing, return.
  auto id = self->GetPointPlacer()->GetCellPicker()->Pick(
      e[0], e[1], e[2], self->GetCurrentRenderer());
  if (id <= 0) {
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

  if (rep->GetNumberOfSeeds() > 1 &&
      self->WidgetState == vtkSeedWidget::PlacingSeeds) {
    self->CompleteInteraction();
  }
}

// void vtkOrthodonticsFACCSeedWidget::MoveAction(vtkAbstractWidget* widget) {
//   auto self = reinterpret_cast<vtkOrthodonticsFACCSeedWidget*>(widget);
//   self->InvokeEvent(vtkCommand::MouseMoveEvent, nullptr);
//   self->RequestCursorShape(VTK_CURSOR_DEFAULT);
// }

vtkOrthodonticsFACCSeedWidget::vtkOrthodonticsFACCSeedWidget() {
  CallbackMapper->SetCallbackMethod(
      vtkCommand::LeftButtonPressEvent, vtkWidgetEvent::AddPoint, this,
      &vtkOrthodonticsFACCSeedWidget::AddPointAction);
  // CallbackMapper->SetCallbackMethod(vtkCommand::MouseMoveEvent,
  //                                   vtkWidgetEvent::Move, this,
  //                                   &vtkOrthodonticsFACCSeedWidget::MoveAction);

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