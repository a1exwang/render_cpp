cmake_minimum_required(VERSION 3.2)
project(render)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)
FIND_PACKAGE(OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

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
        src/objects/Plane.cpp src/utils/BRDF.cpp src/utils/BRDF.h)
#add_compile_options(-pg)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
add_compile_options(-DCV_SSE)
add_compile_options(-DWITH_TBB)
add_compile_options(-DCV_ENABLE_IPP)
add_compile_options(-Wall)
link_libraries(m)
link_libraries(pthread)
add_executable(render ${SOURCE_FILES})
target_link_libraries(render ${OpenCV_LIBS})