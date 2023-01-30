/**
 * @file vtkOrthodonticsContourExtractionFilter.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2023-01-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef VTK_ORTHODONTICS_CONTOUR_EXTRACTION_FILTER_HPP
#define VTK_ORTHODONTICS_CONTOUR_EXTRACTION_FILTER_HPP

// vtk
#include <vtkPolyDataAlgorithm.h>

class vtkOrthodonticsContourExtractionFilter : public vtkPolyDataAlgorithm {
 public:
  static vtkOrthodonticsContourExtractionFilter* New();
  vtkTypeMacro(vtkOrthodonticsContourExtractionFilter, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent) override;

  vtkSetStringMacro(PointDataArrayName);
  vtkGetStringMacro(PointDataArrayName);

 protected:
  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
                  vtkInformationVector* outputVector) override;

  char* PointDataArrayName = nullptr;
};

#endif  // ! VTK_ORTHODONTICS_CONTOUR_EXTRACTION_FILTER_HPP