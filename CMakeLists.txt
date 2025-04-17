cmake_minimum_required(VERSION 3.5)

project(CV_Mid-Term_Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(include)

add_executable(detector
    src/main.cpp
    include/processing.h
    src/processing.cpp
    include/testimage.h
    include/testimage.cpp
)
target_link_libraries(detector ${OpenCV_LIBS})

# For testing
add_executable(test
    test/KAZE_FLANN_matching_homography_Demo.cpp
)
target_link_libraries(test ${OpenCV_LIBS})
