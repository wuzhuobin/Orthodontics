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
#include <vtkCleanPolyData.h>
#include <vtkFeatureEdges.h>
#include <vtkGeometryFilter.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkThreshold.h>

// std
#include <vector>

class vtkOrthodonticsContourExtractionFilter : public vtkPolyDataAlgorithm {
 public:
  static vtkOrthodonticsContourExtractionFilter* New();
  vtkTypeMacro(vtkOrthodonticsContourExtractionFilter, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent) override;

  // PredictedID
  vtkSetStringMacro(PointDataArrayName);
  vtkGetStringMacro(PointDataArrayName);

  vtkSetMacro(Label, int);
  vtkGetMacro(Label, int);

 protected:
  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
                  vtkInformationVector* outputVector) override;

  char* PointDataArrayName = nullptr;
  int Label = 0;
  vtkNew<vtkThreshold> Threshold;
  vtkNew<vtkGeometryFilter> GeometryFilter;
  vtkNew<vtkFeatureEdges> FeatureEdges;
  vtkNew<vtkPolyDataConnectivityFilter> PolyDataConnectivityFilter;
  vtkNew<vtkCleanPolyData> CleanPolyData;
};

#endif  // ! VTK_ORTHODONTICS_CONTOUR_EXTRACTION_FILTER_HPP