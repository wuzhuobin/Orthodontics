/**
 * @file vtkOrthodonticsPCAFilter.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2023-02-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef VTK_ORTHODONTICS_PCA_FILTER_HPP
#define VTK_ORTHODONTICS_PCA_FILTER_HPP

#include <vtkPolyDataAlgorithm.h>

class vtkOrthodonticsPCAFilter : public vtkPolyDataAlgorithm {
 public:
  static vtkOrthodonticsPCAFilter* New();
  vtkTypeMacro(vtkOrthodonticsPCAFilter, vtkPolyDataAlgorithm);
  //   void PrintSelf(ostream &os, vtkIndent indent) override;

 protected:
  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
                  vtkInformationVector* outputVector) override;
};

#endif  // ! VTK_ORTHODONTICS_PCA_FILTER_HPP