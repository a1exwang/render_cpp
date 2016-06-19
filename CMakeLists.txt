cmake_minimum_required(VERSION 3.5)
project(render)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp src/PathTrace.cpp src/PathTrace.h src/Camera.cpp src/Camera.h src/World.cpp src/World.h src/objects/ObjectBase.cpp src/objects/ObjectBase.h src/ObjectManager.cpp src/ObjectManager.h src/utils/Ray.cpp src/utils/Ray.h src/objects/Sphere.cpp src/objects/Sphere.h src/utils/Logger.cpp src/utils/Logger.h)
link_libraries(m)
add_executable(render ${SOURCE_FILES})
target_link_libraries(render ${OpenCV_LIBS})