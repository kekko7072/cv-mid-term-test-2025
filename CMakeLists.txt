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
    src/preprocessing.cpp
    src/processing.cpp
)
target_link_libraries(detector ${OpenCV_LIBS})

add_executable(preprocessing
    src/preprocessing_main.cpp
    src/preprocessing.cpp
)
target_link_libraries(preprocessing ${OpenCV_LIBS})
