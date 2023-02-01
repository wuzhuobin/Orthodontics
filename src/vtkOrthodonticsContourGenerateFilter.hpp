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

#include "vtkOrthodonticsPCACurvatureNormalEstimation.hpp"

// vtk
#include <vtkCleanPolyData.h>
#include <vtkGeometryFilter.h>
#include <vtkNew.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkThreshold.h>

/**
 * @brief
 * @todo The curvature calculation is not correct.
 */
class vtkOrthodonticsContourGenerateFilter : public vtkPolyDataAlgorithm {
 public:
  static vtkOrthodonticsContourGenerateFilter* New();
  vtkTypeMacro(vtkOrthodonticsContourGenerateFilter, vtkPolyDataAlgorithm);
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
  vtkGetMacro(NumberOfExtractedRegions, int);
  vtkSetMacro(ExtractedRegions, int);
  vtkGetMacro(ExtractedRegions, int);

 protected:
  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
                  vtkInformationVector* outputVector) override;

  vtkNew<vtkOrthodonticsPCACurvatureNormalEstimation> CurvatureNormalEstimation;
  vtkNew<vtkThreshold> Threshold;
  double UpperThreshold = std::numeric_limits<double>::infinity();
  double LowerThreshold = -std::numeric_limits<double>::infinity();
  vtkNew<vtkGeometryFilter> GeometryFilter;
  vtkNew<vtkPolyDataConnectivityFilter> PolyDataConnectivityFilter;
  vtkNew<vtkCleanPolyData> CleanPolyData;
  int NumberOfExtractedRegions = 0;
  int ExtractedRegions = -1;
};

#endif  // ! VTK_ORTHODONTICS_CONTOUR_GENERATE_FILTER_HPP