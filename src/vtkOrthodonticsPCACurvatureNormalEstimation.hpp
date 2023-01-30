/**
 * @file vtkOrthodonticsPCACurvatureNormalEstimation.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2023-01-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef VTK_ORTHODONTICS_PCA_CURVATURE_NORMAL_ESTIMATION_HPP
#define VTK_ORTHODONTICS_PCA_CURVATURE_NORMAL_ESTIMATION_HPP

// vtk
#include <vtkPCANormalEstimation.h>

class vtkOrthodonticsPCACurvatureNormalEstimation
    : public vtkPCANormalEstimation {
 public:
  static vtkOrthodonticsPCACurvatureNormalEstimation* New();
  vtkTypeMacro(vtkOrthodonticsPCACurvatureNormalEstimation,
               vtkPCANormalEstimation);

  int RequestData(vtkInformation*, vtkInformationVector**,
                  vtkInformationVector*) override;
};

#endif  // !VTK_ORTHODONTICS_PCA_CURVATURE_NORMAL_ESTIMATION_HPP