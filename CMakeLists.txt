cmake_minimum_required(VERSION 3.2)
project(render)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)

set(SOURCE_FILES
        main.cpp
        src/PathTrace.cpp
        src/Camera.cpp
        src/World.cpp
        src/objects/ObjectBase.cpp
        src/ObjectManager.cpp
        src/utils/Ray.cpp
        src/objects/Sphere.cpp
        src/utils/Logger.cpp
        src/utils/MathUtils.cpp
        src/objects/Plane.cpp)
#add_compile_options(-pg)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
add_compile_options(-Wall)
link_libraries(m)
link_libraries(pthread)
add_executable(render ${SOURCE_FILES})
target_link_libraries(render ${OpenCV_LIBS})