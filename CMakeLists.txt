project(People_Remover)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(src/ SRC_LIST)

find_package(OpenCV 3.3.1 REQUIRED core imgproc highgui opencv_dnn)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
