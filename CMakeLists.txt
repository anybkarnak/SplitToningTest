cmake_minimum_required(VERSION 3.3)
project(split_toning_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(split-toner)

find_package( OpenCV REQUIRED core imgproc objdetect photo )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(SOURCE_FILES main.cpp)
add_executable(split_toning_test ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} split-toner)
add_dependencies(${PROJECT_NAME} split-toner)