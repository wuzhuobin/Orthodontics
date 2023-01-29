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
  //   void PrintSelf(ostream& os, vtkIndent indent) override;

  //   ///@{
  //   /**
  //    * For each sampled point, specify the number of the closest, surrounding
  //    * points used to estimate the normal (the so called k-neighborhood). By
  //    * default 25 points are used. Smaller numbers may speed performance at
  //    the
  //    * cost of accuracy.
  //    */
  //   vtkSetClampMacro(SampleSize, int, 1, VTK_INT_MAX);
  //   vtkGetMacro(SampleSize, int);
  //   ///@}

  //   ///@{
  //   /**
  //    * Specify a point locator. By default a vtkStaticPointLocator is
  //    * used. The locator performs efficient searches to locate points
  //    * around a sample point.
  //    */
  //   void SetLocator(vtkAbstractPointLocator* locator);
  //   vtkGetObjectMacro(Locator, vtkAbstractPointLocator);
  //   ///@}

  //  protected:
  //   vtkOrthodonticsPCACurvatureNormalEstimation();
  //   ~vtkOrthodonticsPCACurvatureNormalEstimation() override;

  //   // IVars
  //   int SampleSize = 25;
  //   vtkAbstractPointLocator* Locator;

  //   int RequestData(vtkInformation*, vtkInformationVector**,
  //                   vtkInformationVector*) override;
  //   int FillInputPortInformation(int port, vtkInformation* info) override;
};

#endif  // !VTK_ORTHODONTICS_PCA_CURVATURE_NORMAL_ESTIMATION_HPP