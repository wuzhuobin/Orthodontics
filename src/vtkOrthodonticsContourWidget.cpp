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
#include <vtkActor.h>
#include <vtkCleanPolyData.h>
#include <vtkCommand.h>
#include <vtkDecimatePolylineFilter.h>
#include <vtkIdList.h>
#include <vtkMapper.h>
#include <vtkObjectFactory.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkPoints.h>
#include <vtkPolyDataCollection.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolygonalSurfaceContourLineInterpolator.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkStripper.h>
#include <vtkTriangleFilter.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkWidgetEvent.h>

vtkStandardNewMacro(vtkOrthodonticsContourWidget);

void vtkOrthodonticsContourWidget::Initialize(vtkActor* prop,
                                              vtkPolyData* points) {
  PolygonalSurfacePointPlacer->RemoveAllProps();
  PolygonalSurfacePointPlacer->AddProp(prop);

  if (prop == nullptr && prop->GetMapper() == nullptr) {
    vtkWarningMacro(<< "Clippee is not valid.");
    return;
  }

  Clippee = vtkPolyData::SafeDownCast(prop->GetMapper()->GetInput());

  if (Clippee == nullptr) {
    vtkWarningMacro(<< "Clippee is not valid.");
    return;
  }

  vtkNew<vtkPolyDataNormals> polyDataNormals;
  polyDataNormals->SetInputData(Clippee);
  polyDataNormals->SetComputeCellNormals(true);
  polyDataNormals->SetComputePointNormals(true);
  polyDataNormals->Update();
  Clippee->ShallowCopy(polyDataNormals->GetOutput());

  // For initialization.
  PolygonalSurfacePointPlacer->GetPolys()->AddItem(Clippee);

  if (points == nullptr || points->GetNumberOfPoints() < 3) {
    return;
  }

  ///@{
  // Find the largest cell.
  vtkNew<vtkStripper> stripper;
  stripper->SetInputData(points);
  stripper->Update();

  vtkNew<vtkDecimatePolylineFilter> decimatePolylineFilter;
  decimatePolylineFilter->SetInputConnection(stripper->GetOutputPort());
  decimatePolylineFilter->SetTargetReduction(0.95);
  decimatePolylineFilter->Update();
  auto intermediate = decimatePolylineFilter->GetOutput();

  vtkIdType cellIdWithMostPoints = 0;
  for (auto cid = 0; cid < intermediate->GetNumberOfCells(); ++cid) {
    if (intermediate->GetCell(cellIdWithMostPoints)->GetNumberOfPoints() <
        intermediate->GetCell(cid)->GetNumberOfPoints()) {
      cellIdWithMostPoints = cid;
    }
  }
  auto cellWithMostPoints = intermediate->GetCell(cellIdWithMostPoints);
  ///@}

  ///@{
  // Reorder the points.
  auto* newCell = cellWithMostPoints->NewInstance();
  points->Reset();
  points->Allocate();
  vtkNew<vtkPoints> newPoints;
  vtkNew<vtkIdList> newIdList;
  for (auto cpid = 0; cpid < cellWithMostPoints->GetNumberOfPoints(); ++cpid) {
    newCell->GetPointIds()->InsertNextId(cpid);
    auto pid = cellWithMostPoints->GetPointId(cpid);
    auto point = intermediate->GetPoint(pid);
    newPoints->InsertNextPoint(point);
    // Update the line.
    auto foundPointId = Clippee->FindPoint(point);
    newIdList->InsertNextId(foundPointId);
  }
  points->InsertNextCell(newCell->GetCellType(), newCell->GetPointIds());
  newCell->Delete();
  points->SetPoints(newPoints);
  ///@}

  vtkContourWidget::Initialize(points, 1, newIdList);
  CloseLoop();
}

vtkPolyData* vtkOrthodonticsContourWidget::Clip() {
  if (Clippee == nullptr) {
    vtkWarningMacro(<< "Clippee is not valid.");
    return nullptr;
  }

  auto contour = GetOrientedGlyphContourRepresentation()
                     ->GetContourRepresentationAsPolyData();
  if (contour == nullptr) {
    vtkWarningMacro(<< "Contour is not valid.");
    return nullptr;
  }

  vtkNew<vtkTriangleFilter> triangleFilter;
  triangleFilter->SetInputData(Clippee);
  triangleFilter->Update();

  auto clippee = triangleFilter->GetOutput();

  auto contourPoints = contour->GetPoints();
  vtkNew<vtkIdList> toBeRemovedPointIds;
  vtkNew<vtkPoints> toBeRemovedPoints;
  vtkNew<vtkIdList> toBeRemovedCellIds;
  clippee->BuildLinks();
  clippee->BuildCells();
  for (auto pid = 0; pid < contourPoints->GetNumberOfPoints(); pid++) {
    auto point = contourPoints->GetPoint(pid);
    toBeRemovedPoints->InsertNextPoint(point);
    auto toBeRemovedPointId = clippee->FindPoint(point);
    toBeRemovedPointIds->InsertNextId(toBeRemovedPointId);
    vtkIdType nCells;
    vtkIdType* cells;
    clippee->GetPointCells(toBeRemovedPointId, nCells, cells);
    for (auto cid = 0; cid < nCells; cid++) {
      toBeRemovedCellIds->InsertUniqueId(cells[cid]);
    }
  }
  auto clippeePoints = clippee->GetPoints();
  vtkNew<vtkPoints> newPoints;
  newPoints->DeepCopy(clippeePoints);

  ClippedClippee = PolyDataPtr::New();
  ClippedClippee->SetPoints(newPoints);
  ClippedClippee->Allocate();
  for (auto cid = 0; cid < clippee->GetNumberOfCells(); cid++) {
    if (toBeRemovedCellIds->FindIdLocation(cid) != -1) {
      continue;
    }
    auto* cell = clippee->GetCell(cid);
    if (cell == nullptr) {
      continue;
    }
    ClippedClippee->InsertNextCell(cell->GetCellType(), cell->GetPointIds());
  }

  vtkNew<vtkCleanPolyData> cleanPolyData;
  cleanPolyData->SetInputData(ClippedClippee);
  cleanPolyData->Update();

  vtkNew<vtkPolyDataConnectivityFilter> polyDataConnectivityFilter;
  polyDataConnectivityFilter->SetInputConnection(
      cleanPolyData->GetOutputPort());
  polyDataConnectivityFilter->SetExtractionMode(VTK_EXTRACT_ALL_REGIONS);
  polyDataConnectivityFilter->Update();
  auto numOfRegions = polyDataConnectivityFilter->GetNumberOfExtractedRegions();
  if (numOfRegions != 2) {
    vtkWarningMacro(<< "Invalid contour.");
    return nullptr;
  }
  polyDataConnectivityFilter->AddSpecifiedRegion(1);
  polyDataConnectivityFilter->SetExtractionMode(VTK_EXTRACT_SPECIFIED_REGIONS);
  polyDataConnectivityFilter->Update();
  ClippedClippee->ShallowCopy(polyDataConnectivityFilter->GetOutput());

  return ClippedClippee;
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