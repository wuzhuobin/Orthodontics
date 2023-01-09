#%%
import vtk
from vtkmodules.vtkIOGeometry import vtkSTLReader
from vtkmodules.vtkRenderingCore import (
    vtkPolyDataMapper,
    vtkActor,
    vtkRenderWindow,
    vtkRenderer,
    vtkRenderWindowInteractor,
)
from vtkmodules.vtkInteractionStyle import vtkInteractorStyleTrackballCamera

#%%
def main():
  print("main")

  stlReader = vtkSTLReader()
  stlReader.SetFileName("../data/Lower+AntagonistScan.stl")
  stlReader.Update()
  print(stlReader.GetOutput())
  
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

if __name__ == '__main__':
    main()
# %%
