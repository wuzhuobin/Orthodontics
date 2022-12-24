/**
 * @file vtkOrthodonticsImplicitPlaneWidget.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-12-24
 *
 * @copyright copyright (c) 2022
 *
 */

#ifndef VTK_ORTHODONTICS_IMPLICIT_PLANE_WIDGET_HPP
#define VTK_ORTHODONTICS_IMPLICIT_PLANE_WIDGET_HPP

// vtk
#include <vtkImplicitPlaneWidget2.h>
class vtkPolyData;

class vtkOrthodonticsImplicitPlaneWidget : public vtkImplicitPlaneWidget2 {
 public:
  static vtkOrthodonticsImplicitPlaneWidget* New();
  vtkTypeMacro(vtkOrthodonticsImplicitPlaneWidget, vtkImplicitPlaneWidget2);

  //   virtual void Initialize(double bounds[6]);
  virtual void Initialize(vtkPolyData* data);
};

#endif  // !VTK_ORTHODONTICS_IMPLICIT_PLANE_WIDGET_HPP