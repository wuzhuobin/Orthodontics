#%%
import vtk
from vtkmodules.vtkIOGeometry import (
  vtkSTLReader,
  vtkSTLWriter,
)
from vtkmodules.vtkIOLegacy import (
  vtkPolyDataWriter,
)
from vtkmodules.vtkRenderingCore import (
  vtkPolyDataMapper,
  vtkActor,
  vtkRenderWindow,
  vtkRenderer,
  vtkRenderWindowInteractor,
)
from vtkmodules.vtkInteractionStyle import (
  vtkInteractorStyleTrackballCamera,
)
from vtkmodules.vtkFiltersGeneral import (
  vtkCurvatures,
)
from vtkmodules.vtkFiltersPoints import (
  vtkPCACurvatureEstimation,
  vtkPCANormalEstimation,
)
from vtkmodules.vtkCommonCore import (
    VTK_DOUBLE,
    vtkDoubleArray,
    vtkFloatArray,
)
import math

#%%
stlReader = vtkSTLReader()
stlReader.SetFileName("../data/Lower+AntagonistScanClipped.stl")
stlReader.Update()

# curvatures = vtkCurvatures()
# curvatures.SetInputConnection(stlReader.GetOutputPort())
# curvatures.SetCurvatureTypeToGaussian()
# curvatures.Update()
# print(curvatures.GetOutput())


pcaCurvature = vtkPCACurvatureEstimation()
pcaCurvature.SetInputConnection(stlReader.GetOutputPort())
pcaCurvature.SetSampleSize(25)
pcaCurvature.Update()


pcaCurvatureArray = pcaCurvature.GetOutput().GetPointData().GetArray("PCACurvature")
stlReader.GetOutput().GetPointData().AddArray(pcaCurvatureArray)

pcaCurvatureMag = vtkFloatArray()
pcaCurvatureMag.SetName("Curvatures")
pcaCurvatureMag.SetNumberOfValues(pcaCurvatureArray.GetNumberOfTuples())
for i in range(0, pcaCurvatureArray.GetNumberOfTuples()):
  curRad = pcaCurvatureArray.GetTuple3(i)
  cur = curRad[0] * curRad[0] + curRad[1] * curRad[1] + curRad[2] * curRad[2]
  cur = math.sqrt(cur)
  cur = 1.0 / cur
  pcaCurvatureMag.SetValue(i, cur)

stlReader.GetOutput().GetPointData().AddArray(pcaCurvatureMag)

pcaNormal = vtkPCANormalEstimation()
pcaNormal.SetInputConnection(stlReader.GetOutputPort())
pcaNormal.SetFlipNormals(True)
pcaNormal.SetSampleSize(25)
pcaNormal.SetNormalOrientationToGraphTraversal()
pcaNormal.Update()

pcaNormalArray = pcaNormal.GetOutput().GetPointData().GetArray("PCANormals")

stlReader.GetOutput().GetPointData().AddArray(pcaNormalArray)

polyDataWriter = vtkPolyDataWriter()
# polyDataWriter.SetInputConnection(stlReader.GetOutputPort())
polyDataWriter.SetInputData(stlReader.GetOutput())
polyDataWriter.SetFileName("../data/curvature.vtk")
polyDataWriter.Write()

#%%
dataMapper = vtkPolyDataMapper()
dataMapper.SetInputConnection(stlReader.GetOutputPort())

dataActor = vtkActor()
dataActor.SetMapper(dataMapper)


renderer = vtkRenderer()
renderer.AddActor(dataActor)

renderWindow = vtkRenderWindow()
renderWindow.AddRenderer(renderer)
renderWindow.SetSize(1024, 512)

style = vtkInteractorStyleTrackballCamera()

interactor = vtkRenderWindowInteractor()
interactor.SetRenderWindow(renderWindow)
interactor.SetInteractorStyle(style)

interactor.Start()
# %%

def main():
  print("main")

if __name__ == '__main__':
    main()
# %%
