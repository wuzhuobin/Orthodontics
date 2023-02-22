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
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>

vtkStandardNewMacro(vtkOrthodonticsPCAFilter);

int vtkOrthodonticsPCAFilter::RequestData(vtkInformation* request,
                                          vtkInformationVector** inputVector,
                                          vtkInformationVector* outputVector) {
  return 1;
}