project(tutorial-image)

cmake_minimum_required(VERSION 2.8)

find_package(VISP REQUIRED)
if(VISP_FOUND)
	INCLUDE(${VISP_USE_FILE})
endif(VISP_FOUND)
#Remplace
#include_directories(${VISP_INCLUDE_DIRS})
#target_link_libraries(tutorial-viewer ${VISP_LIBRARIES})

add_executable(tutorial-viewer tutorial-viewer.cpp)
add_executable(tutorial-grabber-v4l2 tutorial-grabber.cpp)
add_executable(tutorial-grabber-opencv tutorial-grabber-opencv-bis.cpp)
add_executable(tutorial-video tutorial-video-reader.cpp)
#Alternative pour un seul executable
#file(GLOB SRC . *.cpp)
#add_executable(<machin_exe> ${SRC}) 


