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
#include "QOrthodonticsWidget.hpp"
#include "QBridgeVtk.hpp"

// qt
#include <QApplication>

// std
#include <iostream>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QBridgeVtk bridge;
	QOrthodonticsWidget widget;
	widget.show();

	return app.exec();
}