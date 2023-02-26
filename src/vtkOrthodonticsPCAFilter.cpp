/**
 * @file vtkOrthodonticsPCAFilter.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2023-02-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "vtkOrthodonticsPCAFilter.hpp"

// vtk
// #include <vtkArrowSource.h>
#include <vtkDoubleArray.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkLineSource.h>
#include <vtkObjectFactory.h>
#include <vtkPCAStatistics.h>
#include <vtkTable.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

vtkStandardNewMacro(vtkOrthodonticsPCAFilter);

int vtkOrthodonticsPCAFilter::RequestData(vtkInformation* request,
                                          vtkInformationVector** inputVector,
                                          vtkInformationVector* outputVector) {
  // get the info objects
  vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkPolyData* input =
      vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData* output =
      vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkNew<vtkTable> table;
  vtkNew<vtkDoubleArray> arrays[3];
  arrays[0]->SetName("x");
  arrays[1]->SetName("y");
  arrays[2]->SetName("z");
  for (auto&& array : arrays) {
    array->SetNumberOfComponents(1);
    table->AddColumn(array);
  }
  for (auto pid = 0; pid < input->GetNumberOfPoints(); ++pid) {
    auto point = input->GetPoint(pid);
    for (auto i = 0; i < 3; i++) {
      arrays[i]->InsertNextValue(point[i]);
    }
  }

  vtkNew<vtkPCAStatistics> pcaStatistics;
  pcaStatistics->SetInputData(vtkStatisticsAlgorithm::INPUT_DATA, table);
  pcaStatistics->SetColumnStatus("x", 1);
  pcaStatistics->SetColumnStatus("y", 1);
  pcaStatistics->SetColumnStatus("z", 1);
  pcaStatistics->RequestSelectedColumns();
  pcaStatistics->SetDeriveOption(true);
  pcaStatistics->Update();

  // for (auto i = 0; i < 3; ++i) {
  //   vtkNew<vtkDoubleArray> eigenVector;
  //   auto eigenValue = pcaStatistics->GetEigenvalue(i);
  //   std::cerr << "eigen value: " << eigenValue << '\n';
  //   pcaStatistics->GetEigenvector(i, eigenVector);
  //   std::cerr << "eigen vector" << std::to_string(i) << ": ";
  //   for (auto pid = 0; pid < eigenVector->GetNumberOfValues(); pid++) {
  //     std::cerr << eigenVector->GetValue(pid) << '\t';
  //   }
  //   std::cerr << '\n';
  // }

  auto center = input->GetCenter();
  auto eigenValue = pcaStatistics->GetEigenvalue(0);
  vtkNew<vtkDoubleArray> eigenVector;
  pcaStatistics->GetEigenvector(0, eigenVector);
  double point2[] = {eigenVector->GetValue(0), eigenVector->GetValue(1),
                     eigenVector->GetValue(2)};

  vtkNew<vtkLineSource> lineSource;
  lineSource->SetPoint1(0, 0, 0);
  lineSource->SetPoint2(point2);
  lineSource->Update();

  vtkNew<vtkTransform> transform;
  transform->Translate(center);
  transform->Scale(eigenValue, eigenValue, eigenValue);

  vtkNew<vtkTransformPolyDataFilter> polyDataFilter;
  polyDataFilter->SetInputConnection(lineSource->GetOutputPort());
  polyDataFilter->SetTransform(transform);
  polyDataFilter->Update();

  output->ShallowCopy(polyDataFilter->GetOutput());

  return 1;
}