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
#include <vtkDataObject.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkPolyData.h>

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

  Threshold->SetInputData(input);
  Threshold->SetUpperThreshold(UpperThreshold);
  Threshold->SetLowerThreshold(LowerThreshold);
  Threshold->Update();

  return 1;
}
