/**
 * @file QOrthodonticsViewWidget.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-12-21
 *
 * @copyright copyright (c) 2022
 *
 */

#ifndef Q_ORTHODONTICS_VIEW_WIDGET_HPP
#define Q_ORTHODONTICS_VIEW_WIDGET_HPP

// vtk
#include <QVTKOpenGLNativeWidget.h>
class vtkActor;
class vtkPolyData;

class QOrthodonticsViewWidget : public QVTKOpenGLNativeWidget {
  Q_OBJECT

 public:
  explicit QOrthodonticsViewWidget(QWidget *parent = nullptr);

 private:
  vtkActor *addPolyDataFromPath(const std::string &path) const;
  vtkActor *renderPolyData(vtkPolyData *polyData) const;
};

#endif  //! Q_ORTHODONTICS_VIEW_WIDGET_HPP