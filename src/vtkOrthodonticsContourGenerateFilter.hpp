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
  ///@{
  /**
   * Set/get the upper and lower thresholds. The default values are set to
   * +infinity and -infinity, respectively.
   */
  vtkSetMacro(UpperThreshold, double);
  vtkSetMacro(LowerThreshold, double);
  vtkGetMacro(UpperThreshold, double);
  vtkGetMacro(LowerThreshold, double);
  ///@}
 protected:
  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
                  vtkInformationVector* outputVector) override;

  vtkNew<vtkThreshold> Threshold;
  double LowerThreshold = VTK_DOUBLE_MIN;
  double UpperThreshold = VTK_DOUBLE_MAX;
};

#endif  // ! VTK_ORTHODONTICS_CONTOUR_GENERATE_FILTER_HPP