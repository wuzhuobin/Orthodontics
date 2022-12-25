/**
 * @file vtkOrthodonticsImplicitPlaneWidget.cpp
 * @copyright Copyright (c) 2022 Koninklijke Philips N.V. @ER2967
 * @brief
 * @version 0.1
 * @date 2022-12-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "vtkOrthodonticsImplicitPlaneWidget.hpp"

// vtk
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>

vtkStandardNewMacro(vtkOrthodonticsImplicitPlaneWidget);

void vtkOrthodonticsImplicitPlaneWidget::Initialize(vtkPolyData *data) {
  auto *representation = GetImplicitPlaneRepresentation();
  representation->PlaceWidget(data->GetBounds());
}

vtkPlane *vtkOrthodonticsImplicitPlaneWidget::GetImplicitPlane() {
  return GetImplicitPlaneRepresentation()->GetUnderlyingPlane();
}

vtkOrthodonticsImplicitPlaneWidget::vtkOrthodonticsImplicitPlaneWidget() {
  CreateDefaultRepresentation();
  auto *representation = GetImplicitPlaneRepresentation();
  representation->SetPlaceFactor(1.0);
}