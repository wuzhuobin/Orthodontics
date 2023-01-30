/**
 * @file vtkOrthodonticsPCACurvatureNormalEstimation.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2023-01-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "vtkOrthodonticsPCACurvatureNormalEstimation.hpp"

// vtk
#include <vtkObjectFactory.h>
// #include <vtkStaticPointLocator.h>

vtkStandardNewMacro(vtkOrthodonticsPCACurvatureNormalEstimation);
// vtkCxxSetObjectMacro(vtkOrthodonticsPCACurvatureNormalEstimation, Locator,
//                      vtkAbstractPointLocator);

// vtkOrthodonticsPCACurvatureNormalEstimation::
//     vtkOrthodonticsPCACurvatureNormalEstimation()
//     : Locator(vtkStaticPointLocator::New()) {}

// vtkOrthodonticsPCACurvatureNormalEstimation::
//     ~vtkOrthodonticsPCACurvatureNormalEstimation() {
//   SetLocator(nullptr);
// }

// void vtkOrthodonticsPCACurvatureNormalEstimation::PrintSelf(ostream& os,
//                                                             vtkIndent indent)
//                                                             {
//   this->Superclass::PrintSelf(os, indent);

//   os << indent << "Sample Size: " << this->SampleSize << "\n";
//   os << indent << "Locator: " << this->Locator << "\n";
// }