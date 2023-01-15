/*
 * Software License Agreement (BSD License)
 *
 * Point Cloud Library (PCL) - www.pointclouds.org
 * Copyright (c) 2009-2011, Willow Garage, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following
 *   disclaimer in the documentation and/or other materials provided
 *   with the distribution.
 * * Neither the name of Willow Garage, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 *
 */

// pcl
#include <pcl/features/normal_3d.h>
#include <pcl/features/principal_curvatures.h>
// #include <pcl/io/pcd_io.h>
// #include <pcl/conversions.h>
#include <pcl/io/vtk_lib_io.h>
#include <pcl/point_types.h>

// vtk
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkNew.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkSTLReader.h>

#include <iostream>
#include <vector>

int main(int, char** argv) {
  std::string filename =
      "C:/ccode/Orthodontics/data/Lower+AntagonistScanClipped.stl";
  std::cout << "Reading " << filename << '\n';

  vtkNew<vtkSTLReader> stlReader;
  stlReader->SetFileName(filename.c_str());
  stlReader->Update();
  auto data = stlReader->GetOutput();

  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
  pcl::io::vtkPolyDataToPointCloud(stlReader->GetOutput(), *cloud);

  pcl::PointCloud<pcl::Normal>::Ptr cloud_with_normals(
      new pcl::PointCloud<pcl::Normal>);
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(
      new pcl::search::KdTree<pcl::PointXYZ>);

  // Compute the normals
  pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normal_estimation;
  normal_estimation.setInputCloud(cloud);
  normal_estimation.setSearchMethod(tree);
  normal_estimation.setKSearch(20);
  normal_estimation.compute(*cloud_with_normals);

  vtkNew<vtkFloatArray> normals;
  normals->SetNumberOfComponents(3);
  normals->SetNumberOfTuples(cloud_with_normals->size());
  normals->SetName("Normals");
  vtkNew<vtkFloatArray> curvatures;
  curvatures->SetNumberOfValues(cloud_with_normals->size());
  curvatures->SetName("Curvatures");

  for (auto id = 0; id < cloud_with_normals->size(); ++id) {
    normals->SetTuple(id, cloud_with_normals->at(id).normal);
    curvatures->SetValue(id, cloud_with_normals->at(id).curvature);
  }

  data->GetPointData()->AddArray(curvatures);
  data->GetPointData()->AddArray(normals);
  // std::cout << *normals;
  // std::cout << *curvatures;
  // std::cout << *data;

  vtkNew<vtkPolyDataWriter> polyDataWriter;
  polyDataWriter->SetInputData(data);
  polyDataWriter->SetFileName(
      "C:/ccode/Orthodontics/data/Lower+AntagonistScanClippedCurvatures.vtk");
  polyDataWriter->Write();

  std::cout << "Done!\n";
  // Setup the principal curvatures computation
  //   pcl::PrincipalCurvaturesEstimation<pcl::PointXYZ, pcl::Normal,
  //                                      pcl::PrincipalCurvatures>
  //       principal_curvatures_estimation;

  //   // Provide the original point cloud (without normals)
  //   principal_curvatures_estimation.setInputCloud(cloud);

  //   // Provide the point cloud with normals
  //   principal_curvatures_estimation.setInputNormals(cloud_with_normals);

  //   // Use the same KdTree from the normal estimation
  //   principal_curvatures_estimation.setSearchMethod(tree);
  //   principal_curvatures_estimation.setRadiusSearch(1.0);

  //   // Actually compute the principal curvatures
  //   pcl::PointCloud<pcl::PrincipalCurvatures>::Ptr principal_curvatures(
  //       new pcl::PointCloud<pcl::PrincipalCurvatures>());
  //   principal_curvatures_estimation.compute(*principal_curvatures);

  //   std::cout << "output points.size (): " <<
  //   principal_curvatures->points.size()
  //             << std::endl;

  //   // Display and retrieve the shape context descriptor vector for the 0th
  //   for (auto point : principal_curvatures->points) {
  //     std::cout << point << std::endl;
  //   }

  return 0;
}