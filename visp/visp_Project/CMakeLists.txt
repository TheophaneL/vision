project(Visp_Project)
cmake_minimum_required(VERSION 2.8)

file(GLOB SRC_LIST
    "*.hpp"
    "*.cpp")

find_package(VISP REQUIRED)
if(VISP_FOUND)
    INCLUDE(${VISP_USE_FILE})
endif(VISP_FOUND)


#aux_source_directory(. SRC_LIST)
#add_executable(${PROJECT_NAME} ${SRC_LIST})
add_executable(main main.cpp blob-tracking-multiple.cpp line-tracking.cpp ellipse-tracking.cpp model-edge-tracking.cpp)
add_executable(cube-tracking cube-tracking.cpp)
add_executable(tutoriel-blob-tracker-live tutorial-blob-tracker-live-v4l2.cpp)
add_executable(tutoriel-klt-tracker-live tutorial-klt-tracker-live-v4l2.cpp)
add_executable(visp-mbt-tracker visp-mbt-tracker.cpp)
add_executable(visp-calib visp-calib-intrinsic.cpp)
add_executable(camDisto camDisto.cpp)


# Conflit entre Opencv 2.4 et 3.0
#find_package(OpenCV REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})
#target_link_libraries(tutorial-klt-tracker ${OpenCV_LIBS})

