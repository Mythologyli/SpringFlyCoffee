cmake_minimum_required(VERSION 3.0)
project(Latte)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Qt5 COMPONENTS Core REQUIRED)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(RockX REQUIRED)
include_directories(${RockX_INCLUDE_DIRS})

set(SOURCES
        main.cpp
        face_recognition.cpp
        camera/camera.cpp
        rockx_face/rockx_face.cpp
        )

add_executable(Latte ${SOURCES})

target_link_libraries(Latte Qt5::Core)
target_link_libraries(Latte ${OpenCV_LIBS})
target_link_libraries(Latte ${RockX_LIBS})
