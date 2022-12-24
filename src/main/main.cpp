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

// std
#include <iostream>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QOrthodonticsWidget widget;
  widget.show();
  QOrthodonticsViewWidget view;
  view.show();
  view.resize(1024, 1024);
  QBridgeVtk bridge;
  bridge.setViewWidget(&view);
  bridge.setWidget(&widget);
  bridge.setupConnection();

  return app.exec();
}