cmake_minimum_required(VERSION 2.8)
project( tracking )
find_package( OpenCV REQUIRED )

set(CMAKE_BUILD_TYPE Debug)


set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs -Wall -Wextra -Wno-unused -pedantic")

set(SOURCES
    main.cpp
    CoordinateConverter.cpp
    Pool.cpp
    objectTracking/Flooder.h
    objectTracking/Flooder.cpp
    objectTracking/Vec3.cpp
    objectTracking/Vec3.h
    objectTracking/ColorTracker.h
    objectTracking/ColorTracker.cpp
    objectTracking/ImgFunc.h
    objectTracking/ImgFunc.cpp
    objectTracking/BinMap.h
    objectTracking/BinMap.cpp
    objectTracking/Vec2.h
    objectTracking/Vec2.cpp
    )
add_executable( tracking ${SOURCES} )
target_link_libraries( tracking ${OpenCV_LIBS} )
