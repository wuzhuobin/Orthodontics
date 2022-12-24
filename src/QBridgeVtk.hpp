/**
 * @file QBridgeVtk.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-12-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef Q_BRIDGE_VTK_HPP
#define Q_BRIDGE_VTK_HPP

// qt
#include <QObject>
class QOrthodonticsViewWidget;
class QOrthodonticsWidget;

// vtk
#include <vtkContourWidget.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkNew.h>

class QBridgeVtk : public QObject {
  Q_OBJECT

 public:
  void setViewWidget(QOrthodonticsViewWidget* viewWidget);
  void setWidget(QOrthodonticsWidget* widget);
  void setupConnection();

 private:
  QOrthodonticsViewWidget* mViewWidget;
  QOrthodonticsWidget* mWidget;

  vtkNew<vtkContourWidget> mContourWidget;
  vtkNew<vtkImplicitPlaneWidget2> mImplicitPlaneWidget2;

  void enableInteractorObserver(vtkInteractorObserver* observer, bool enabled);
};

#endif  //! Q_BRIDGE_VTK_HPP