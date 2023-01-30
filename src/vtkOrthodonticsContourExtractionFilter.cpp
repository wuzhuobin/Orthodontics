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
#include <vtkObjectFactory.h>

vtkStandardNewMacro(vtkOrthodonticsContourExtractionFilter);

void vtkOrthodonticsContourExtractionFilter::PrintSelf(ostream& os,
                                                       vtkIndent indent) {
  Superclass::PrintSelf(os, indent);
}

int vtkOrthodonticsContourExtractionFilter::RequestData(
    vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) {
  return Superclass::RequestData(request, inputVector, outputVector);
}
