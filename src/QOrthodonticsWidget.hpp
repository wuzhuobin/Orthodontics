/**
 * @file QOrthodonticsWidget.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-12-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef Q_ORTHODONTICS_WIDGET_HPP
#define Q_ORTHODONTICS_WIDGET_HPP

#include "ui_QOrthodonticsWidget.h"

// qt
#include <QWidget>

class QOrthodonticsWidget : public QWidget, Ui::QOrthodonticsWidget
{
	Q_OBJECT

public:
	explicit QOrthodonticsWidget(QWidget *parent = nullptr);
};

#endif // !Q_ORTHODONTICS_WIDGET_HPP