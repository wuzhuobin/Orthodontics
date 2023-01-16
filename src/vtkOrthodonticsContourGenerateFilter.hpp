/**
 * @file vtkOrthodonticsContourGenerateFilter.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2023-01-16
 *
 * @copyright copyright (c) 2022
 *
 */

#ifndef VTK_ORTHODONTICS_CONTOUR_GENERATE_FILTER_HPP
#define VTK_ORTHODONTICS_CONTOUR_GENERATE_FILTER_HPP

// vtk
#include <vtkNew.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkThreshold.h>

class vtkOrthodonticsContourGenerateFilter : public vtkPolyDataAlgorithm {
 public:
 protected:
  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
                  vtkInformationVector* outputVector) override;

  vtkNew<vtkThreshold> Threshold;
};

#endif  // ! VTK_ORTHODONTICS_CONTOUR_GENERATE_FILTER_HPP