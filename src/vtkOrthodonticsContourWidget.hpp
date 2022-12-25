/**
 * @file vtkOrthodonticsContourWidget.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-12-25
 *
 * @copyright copyright (c) 2022
 *
 */

#ifndef VTK_ORTHODONTICS_CONTOUR_WIDGET_HPP
#define VTK_ORTHODONTICS_CONTOUR_WIDGET_HPP

// vtk
#include <vtkContourWidget.h>
#include <vtkSmartPointer.h>
class vtkOrientedGlyphContourRepresentation;
class vtkPolygonalSurfacePointPlacer;
class vtkProp;

class vtkOrthodonticsContourWidget : public vtkContourWidget {
 public:
  static vtkOrthodonticsContourWidget* New();
  vtkTypeMacro(vtkOrthodonticsContourWidget, vtkContourWidget);
  virtual void Initialize(vtkProp* prop);

 protected:
  vtkOrthodonticsContourWidget();

 private:
  using PolygonalSurfacePointPlacerPtr =
      vtkSmartPointer<vtkPolygonalSurfacePointPlacer>;
  PolygonalSurfacePointPlacerPtr PolygonalSurfacePointPlacer;

  vtkOrientedGlyphContourRepresentation*
  GetOrientedGlyphContourRepresentation();
};

#endif  //! VTK_ORTHODONTICS_CONTOUR_WIDGET_HPP