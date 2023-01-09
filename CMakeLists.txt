cmake_minimum_required(VERSION 3.23)

project(Orthodontics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(VTK REQUIRED)

file(GLOB INC ${CMAKE_SOURCE_DIR}/src/*.h[pp|xx]?)
file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*.c[pp|xx]?)
file(GLOB UI ${CMAKE_SOURCE_DIR}/src/*.ui)


add_library(${PROJECT_NAME})

target_sources(
	${PROJECT_NAME}
	PUBLIC ${INC} ${SRC}
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_BINARY_DIR}/src
)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC Qt5::Core Qt5::Widgets ${VTK_LIBRARIES}
)

vtk_module_autoinit(
  TARGETS ${PROJECT_NAME}
  MODULES ${VTK_LIBRARIES}
)

set(MAIN_SRC ${CMAKE_SOURCE_DIR}/src/main/main.cpp)

add_executable(${PROJECT_NAME}Main ${MAIN_SRC})

target_link_libraries(
	${PROJECT_NAME}Main
	PUBLIC ${PROJECT_NAME}
)