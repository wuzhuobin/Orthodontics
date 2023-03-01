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

#include "QOrthodonticsContourControllerWidget.hpp"
#include "QOrthodonticsFillHolesControllerWidget.hpp"
#include "QOrthodonticsGingivalLineGenerateControllerWidget.hpp"
#include "QOrthodonticsImplicitPlaneControllerWidget.hpp"
#include "QOrthodonticsViewWidget.hpp"
#include "QOrthodonticsWidget.hpp"
#include "vtkOrthodonticsContourExtractionFilter.hpp"
#include "vtkOrthodonticsContourGenerateFilter.hpp"
#include "vtkOrthodonticsContourWidget.hpp"
#include "vtkOrthodonticsFACCSeedWidget.hpp"
#include "vtkOrthodonticsImplicitPlaneWidget.hpp"
#include "vtkOrthodonticsPCAFilter.hpp"

// qt
#include <QObject>

// vtk
#include <vtkNew.h>

// std
#include <array>

class QBridgeVtk : public QObject {
  Q_OBJECT

 public:
  explicit QBridgeVtk(QOrthodonticsViewWidget& viewWidget,
                      QOrthodonticsWidget& widget, QObject* parent = nullptr);

 private:
  void setupConnection();
  void setupOrthodonticsImplicitPlaneControllerWidget();
  void setupOrthodonticsContourControllerWidget();
  void setupOrthodonticsFillHoles();
  void setupOrthodonticsGingivalLine();
  void enableInteractorObserver(vtkInteractorObserver* observer, bool enabled);

  static constexpr int GNumberOfTeeth = 32;
  QOrthodonticsImplicitPlaneControllerWidget mImplicitPlaneControllerWidget;
  QOrthodonticsContourControllerWidget mContourControllerWidget;
  QOrthodonticsFillHolesController mFillHolsController;
  QOrthodonticsGingivalLineGenerateControllerWidget
      mGingivalLineGenerateControllerWidget;
  QOrthodonticsViewWidget& mViewWidget;
  QOrthodonticsWidget& mWidget;

  vtkNew<vtkOrthodonticsImplicitPlaneWidget> mImplicitPlaneWidget2;
  vtkNew<vtkOrthodonticsContourGenerateFilter> mGenerateContour;
  std::array<vtkNew<vtkOrthodonticsContourExtractionFilter>, GNumberOfTeeth>
      mContourExtractionFilters;
  std::array<vtkNew<vtkOrthodonticsContourWidget>, 32> mContourWidgets;
  std::array<vtkNew<vtkOrthodonticsPCAFilter>, 32> mPCAFilters;
  std::array<vtkNew<vtkOrthodonticsFACCSeedWidget>, 32> mFACCSeedWidgets;
};

#endif  //! Q_BRIDGE_VTK_HPP