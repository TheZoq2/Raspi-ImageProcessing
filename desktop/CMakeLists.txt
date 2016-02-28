cmake_minimum_required(VERSION 2.8)
project( tracking )

LIST(APPEND CMAKE_MODULE_PATH "/usr/local/lib/cmake")

find_package( OpenCV REQUIRED )

set(CMAKE_BUILD_TYPE Debug)



set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs -Wall -Wextra -Wno-unused -pedantic")

set(LIB_SOURCES
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

    Performance.cpp
    )

set(LIB_NAME "TrackingCode")
add_library(${LIB_NAME} ${LIB_SOURCES})

set(TRACKING_SOURCES
    main.cpp
    Tracker.cpp
    )
add_executable( tracking ${TRACKING_SOURCES} )
target_link_libraries( tracking ${OpenCV_LIBS} ${LIB_NAME})

if(BUILD_RASPI)

    find_package(raspicam REQUIRED)
    if( raspicam_CV_FOUND )
        target_link_libraries(tracking ${raspicam_CV_LIBS})
    else()
        MESSAGE(FATAL_ERROR "Raspicam not found")
    endif()
endif()
