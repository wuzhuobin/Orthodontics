/**
 * @file QSaveLoadUtil.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-12-27
 *
 * @copyright copyright (c) 2022
 *
 */

#ifndef Q_SAVE_LOAD_UTIL_HPP
#define Q_SAVE_LOAD_UTIL_HPP

// qt
#include <QObject>

// std
#include <functional>

// vtk
#include <vtkNew.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
class vtkPolyData;

class QSaveLoadUtil : public QObject {
  Q_OBJECT

 public:
  static const QSaveLoadUtil& instance();

  [[nodiscard]] bool savePolyData(vtkPolyData* data, const QString& name) const;
  [[nodiscard]] vtkPolyData* loadPolyData(const QString& name) const;

 private:
  vtkNew<vtkPolyDataReader> mPolyDataReader;
  vtkNew<vtkPolyDataWriter> mPolyDataWriter;
};

#endif  //! Q_SAVE_LOAD_UTIL_HPP