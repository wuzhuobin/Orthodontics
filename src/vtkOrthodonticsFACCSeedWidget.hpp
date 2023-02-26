/**
 * @file vtkOrthodonticsFACCSeedWidget.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2023-02-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef VTK_ORTHODONTICS_FACCS_SEED_WIDGET_HPP
#define VTK_ORTHODONTICS_FACCS_SEED_WIDGET_HPP

// vtk
#include <vtkSeedWidget.h>
class vtkActor;
class vtkPolygonalSurfacePointPlacer;

class vtkOrthodonticsFACCSeedWidget : public vtkSeedWidget {
 public:
  static vtkOrthodonticsFACCSeedWidget* New();
  virtual void Initialize(vtkActor* prop);

 protected:
  vtkOrthodonticsFACCSeedWidget();
  vtkPolygonalSurfacePointPlacer* GetPointPlacer();
};

#endif  // ! VTK_ORTHODONTICS_FACCS_SEED_WIDGET_HPP
