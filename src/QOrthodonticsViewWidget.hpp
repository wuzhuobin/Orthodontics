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

// std
#include <tuple>

// vtk
#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
class vtkActor;
class vtkPolyData;
class vtkDataSet;
class vtkAbstractMapper3D;
class vtkProp3D;

// qt
#include <QMap>

class QOrthodonticsViewWidget : public QVTKOpenGLNativeWidget {
  Q_OBJECT

 public:
  explicit QOrthodonticsViewWidget(QWidget *parent = nullptr);
  vtkDataSet *getDataSet(const QString &name) const;
  template <typename T>
  T *getDataSet(const QString &name) const {
    return T::SafeDownCast(getDataSet(name));
  }
  vtkAbstractMapper3D *getMapper(const QString &name) const;
  template <typename T>
  T *getMapper(const QString &name) const {
    return T::SafeDownCast(getmapper(name));
  }
  vtkProp3D *getProp(const QString &name) const;
  template <typename T>
  T *getProp(const QString &) const {
    return T::SafeDownCast(getProp(name));
  }
  void addPolyData(const QString &name, vtkPolyData *polyData);

 private:
  vtkActor *addPolyDataFromPath(const QString &path);
  vtkActor *renderPolyData(const QString &name, vtkPolyData *polyData);

  using DataSetPtr = vtkSmartPointer<vtkDataSet>;
  using MapperPtr = vtkSmartPointer<vtkAbstractMapper3D>;
  using PropPtr = vtkSmartPointer<vtkProp3D>;
  using DataTuple = std::tuple<DataSetPtr, MapperPtr, PropPtr>;

  // using DataBase = QMap<QString, vtkSmartPointer<vtkDataSet>>;
  using DataBase = QMap<QString, DataTuple>;
  DataBase mDataBase;
};

#endif  //! Q_ORTHODONTICS_VIEW_WIDGET_HPP