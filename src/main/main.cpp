/**
 * @file main.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-12-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "QBridgeVtk.hpp"
#include "QOrthodonticsViewWidget.hpp"
#include "QOrthodonticsWidget.hpp"

// qt
#include <QApplication>

// vtk
#include <vtkProp3D.h>

// std
#include <iostream>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QOrthodonticsWidget widget;
  widget.show();
  QOrthodonticsViewWidget view;
  ///@{
  ///< tesing
  const auto *lowerPath = "C:/ccode/Orthodontics/data/Lower+AntagonistScan.stl";
  view.addPolyData(lowerPath);
  const auto *lowerClippedPath =
      "C:/ccode/Orthodontics/data/Lower+AntagonistScanClipped.stl";
  view.addPolyData(lowerClippedPath);
  view.getProp("Lower+AntagonistScanClipped")->SetVisibility(false);
  const auto *lowerClippedCurvaturesPath =
      "C:/ccode/Orthodontics/data/Lower+AntagonistScanClippedCurvatures.vtk";
  view.addPolyData(lowerClippedCurvaturesPath);
  view.getProp("Lower+AntagonistScanClippedCurvatures")->SetVisibility(false);
  ///@}
  view.show();
  view.resize(1024, 1024);
  QBridgeVtk bridge(view, widget);

  return app.exec();
}