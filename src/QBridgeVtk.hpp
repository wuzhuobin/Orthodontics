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

#include "QOrthodonticsImplicitPlaneControllerWidget.hpp"
#include "QOrthodonticsViewWidget.hpp"
#include "QOrthodonticsWidget.hpp"
#include "vtkOrthodonticsContourWidget.hpp"
#include "vtkOrthodonticsImplicitPlaneWidget.hpp"

// qt
#include <QObject>

// vtk
#include <vtkNew.h>

class QBridgeVtk : public QObject {
  Q_OBJECT

 public:
  void setViewWidget(QOrthodonticsViewWidget* viewWidget);
  void setWidget(QOrthodonticsWidget* widget);
  void setupConnection();

 private:
  QOrthodonticsImplicitPlaneControllerWidget mImplicitPlaneControllerWidget;
  QOrthodonticsViewWidget* mViewWidget;
  QOrthodonticsWidget* mWidget;

  vtkNew<vtkOrthodonticsContourWidget> mContourWidget;
  vtkNew<vtkOrthodonticsImplicitPlaneWidget> mImplicitPlaneWidget2;

  void enableInteractorObserver(vtkInteractorObserver* observer, bool enabled);
};

#endif  //! Q_BRIDGE_VTK_HPP