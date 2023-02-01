/**
 * @file vtkOrthodonticsContourExtractionFilter.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2023-01-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "vtkOrthodonticsContourExtractionFilter.hpp"

// vtk
#include <vtkDataObject.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>

vtkStandardNewMacro(vtkOrthodonticsContourExtractionFilter);

void vtkOrthodonticsContourExtractionFilter::PrintSelf(ostream& os,
                                                       vtkIndent indent) {
  Superclass::PrintSelf(os, indent);
}

int vtkOrthodonticsContourExtractionFilter::RequestData(
    vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) {
  auto* inInfo = inputVector[0]->GetInformationObject(0);
  auto* input =
      vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));

  auto* outInfo = outputVector->GetInformationObject(0);
  auto* output =
      vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  Threshold->SetInputData(input);
  Threshold->SetThresholdFunction(vtkThreshold::THRESHOLD_BETWEEN);
  Threshold->SetUpperThreshold(Label);
  Threshold->SetLowerThreshold(Label);
  Threshold->SetInputArrayToProcess(
      0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, PointDataArrayName);
  Threshold->Update();

  GeometryFilter->SetInputConnection(Threshold->GetOutputPort());
  GeometryFilter->Update();

  FeatureEdges->SetInputConnection(GeometryFilter->GetOutputPort());
  FeatureEdges->SetFeatureEdges(false);
  FeatureEdges->SetNonManifoldEdges(false);
  FeatureEdges->SetBoundaryEdges(true);
  FeatureEdges->Update();

  PolyDataConnectivityFilter->SetInputConnection(FeatureEdges->GetOutputPort());
  PolyDataConnectivityFilter->SetExtractionModeToLargestRegion();
  PolyDataConnectivityFilter->Update();

  CleanPolyData->SetInputConnection(
      PolyDataConnectivityFilter->GetOutputPort());
  CleanPolyData->Update();

  output->ShallowCopy(CleanPolyData->GetOutput());

  return Superclass::RequestData(request, inputVector, outputVector);
}
