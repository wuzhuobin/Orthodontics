cmake_minimum_required(VERSION 3.23)

project(pcl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(PCL COMPONENTS common io kdtree search features REQUIRED)
find_package(FLANN REQUIRED)

add_executable(${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC ${PCL_INCLUDE_DIRS} ${FLANN_INCLUDE_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC ${PCL_LIBRARIES}
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC ${PCL_DEFINITIONS}
)