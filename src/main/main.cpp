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
  const auto *lowerPath = "C:/ccode/Orthodontics/data/Data.vtk";
  view.addPolyData(lowerPath);
  const auto *lowerClippedPath = "C:/ccode/Orthodontics/data/DataClipped.vtk";
  view.addPolyData(lowerClippedPath);
  view.getProp("DataClipped")->SetVisibility(false);
  for (auto i = 0; i < 14; ++i) {
    view.addPolyData("C:/ccode/Orthodontics/data/Tooth" + QString::number(i) +
                     ".vtk");
    view.getProp("Tooth" + QString::number(i))->SetVisibility(false);
  }
  ///@}
  view.show();
  view.resize(1024, 1024);
  QBridgeVtk bridge(view, widget);

  return app.exec();
}