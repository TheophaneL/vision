cmake_minimum_required(VERSION 2.8)
project( bow )

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable( main bow.cpp )
add_executable( mainUse bowUse.cpp)
add_executable(matcheur matcheur.cpp)

target_link_libraries( main ${OpenCV_LIBRARIES} )
target_link_libraries( mainUse ${OpenCV_LIBRARIES})
target_link_libraries(matcheur ${OpenCV_LIBRARIES})
