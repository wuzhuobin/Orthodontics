/**
 * @file vtkOrthodonticsContourGenerateFilter.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2023-01-16
 *
 * @copyright copyright (c) 2022
 *
 */

#include "vtkOrthodonticsContourGenerateFilter.hpp"

// vtk
#include <vtkAppendPolyData.h>
#include <vtkDataObject.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>

// std

vtkStandardNewMacro(vtkOrthodonticsContourGenerateFilter);

int vtkOrthodonticsContourGenerateFilter::RequestData(
    vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) {
  // get the info objects
  vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkPolyData* input =
      vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData* output =
      vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  CurvatureNormalEstimation->SetInputData(input);
  CurvatureNormalEstimation->SetSampleSize(10);
  CurvatureNormalEstimation->Update();

  Threshold->SetInputConnection(CurvatureNormalEstimation->GetOutputPort());
  Threshold->SetThresholdFunction(vtkThreshold::THRESHOLD_BETWEEN);
  Threshold->SetUpperThreshold(UpperThreshold);
  Threshold->SetLowerThreshold(LowerThreshold);
  Threshold->SetInputArrayToProcess(
      0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "PCACurvature");
  Threshold->Update();

  GeometryFilter->SetInputConnection(Threshold->GetOutputPort());
  GeometryFilter->Update();

  PolyDataConnectivityFilter->SetInputConnection(
      GeometryFilter->GetOutputPort());
  PolyDataConnectivityFilter->SetExtractionModeToAllRegions();
  PolyDataConnectivityFilter->Update();
  NumberOfExtractedRegions =
      PolyDataConnectivityFilter->GetNumberOfExtractedRegions();
  if (ExtractedRegions >= 0) {
    auto morePointsFirst = [](vtkPolyData* left, vtkPolyData* right) {
      return left->GetPoints()->GetNumberOfPoints() >
             right->GetPoints()->GetNumberOfPoints();
    };
    std::vector<vtkSmartPointer<vtkPolyData>> allRegions;

    PolyDataConnectivityFilter->SetExtractionModeToSpecifiedRegions();
    for (auto id = 0; id < NumberOfExtractedRegions; id++) {
      PolyDataConnectivityFilter->InitializeSpecifiedRegionList();
      PolyDataConnectivityFilter->AddSpecifiedRegion(id);
      PolyDataConnectivityFilter->Update();

      CleanPolyData->SetInputData(PolyDataConnectivityFilter->GetOutput());
      CleanPolyData->Update();

      allRegions.push_back(vtkSmartPointer<vtkPolyData>::New());
      allRegions[id]->ShallowCopy(CleanPolyData->GetOutput());
    }
    std::nth_element(allRegions.begin(), allRegions.begin() + ExtractedRegions,
                     allRegions.end(), morePointsFirst);
    vtkNew<vtkAppendPolyData> appendPolyData;
    for (auto id = 0; id <= ExtractedRegions; id++) {
      appendPolyData->AddInputData(allRegions[id]);
    }
    appendPolyData->Update();
    output->ShallowCopy(appendPolyData->GetOutput());
  } else {
    output->ShallowCopy(PolyDataConnectivityFilter->GetOutput());
  }

  return 1;
}
