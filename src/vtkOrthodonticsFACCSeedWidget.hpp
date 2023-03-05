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
class vtkPolyData;
class vtkPolygonalSurfacePointPlacer;

class vtkOrthodonticsFACCSeedWidget : public vtkSeedWidget {
 public:
  vtkTypeMacro(vtkOrthodonticsFACCSeedWidget, vtkSeedWidget);
  static vtkOrthodonticsFACCSeedWidget* New();
  void SetEnabled(int enabled) override;
  virtual void Initialize(vtkActor* prop, vtkPolyData* facc = nullptr);

  virtual void CompleteInteraction() override;
  virtual void RestartInteraction() override;

 protected:
  static void AddPointAction(vtkAbstractWidget* widget);

  vtkOrthodonticsFACCSeedWidget();
  vtkPolygonalSurfacePointPlacer* GetPointPlacer();

  static constexpr int GMaxNumberOfSeeds = 2;
  vtkPolyData* Tooth = nullptr;
  vtkPolyData* Facc = nullptr;
};

#endif  // ! VTK_ORTHODONTICS_FACCS_SEED_WIDGET_HPP
