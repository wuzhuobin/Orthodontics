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

class QBridgeVtk : public QObject {
  Q_OBJECT

 public:
  void setViewWidget(QOrthodonticsViewWidget* viewWidget);
  void setWidget(QOrthodonticsWidget* widget);
  void setupConnection();

 private:
  QOrthodonticsViewWidget* mViewWidget;
  QOrthodonticsWidget* mWidget;
};

#endif  //! Q_BRIDGE_VTK_HPP