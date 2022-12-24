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
#include <vtkSmartPointer.h>
class vtkActor;
class vtkPolyData;
class vtkDataSet;

// qt
#include <QMap>

class QOrthodonticsViewWidget : public QVTKOpenGLNativeWidget {
  Q_OBJECT

 public:
  explicit QOrthodonticsViewWidget(QWidget *parent = nullptr);
  vtkDataSet *getDataSet(const QString &name);
  template <typename T>
  T *getDataSet(const QString &name) {
    return T::SafeDownCast(getDataSet(name));
  }

 private:
  vtkActor *addPolyDataFromPath(const QString &path);
  vtkActor *renderPolyData(vtkPolyData *polyData) const;

  using DataBase = QMap<QString, vtkSmartPointer<vtkDataSet>>;
  DataBase mDataBase;
};

#endif  //! Q_ORTHODONTICS_VIEW_WIDGET_HPP