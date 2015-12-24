####################################
cmake_minimum_required (VERSION 2.8) 
project (raspicam_test)

#set( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} /usr/local/lib/cmake")
LIST(APPEND CMAKE_MODULE_PATH "/usr/local/lib/cmake") 

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs -Wall -Wextra -Wno-unused -pedantic")


find_package(raspicam REQUIRED)
if( raspicam_CV_FOUND )
else()
    MESSAGE(FATAL_ERROR "Raspicam not found")
    endif()

find_package(OpenCV)
IF  ( OpenCV_FOUND)
MESSAGE(STATUS "COMPILING OPENCV TESTS")

set(SOURCES 
    main.cpp 
    objectTracking/Vec2.cpp 
    objectTracking/Vec3.cpp 
    objectTracking/ColorTracker.cpp 
    objectTracking/ImgFunc.cpp 
    objectTracking/BinMap.cpp
    objectTracking/Flooder.cpp
    )

#add_executable (objectTracking main.cpp)  
add_executable (objectTracking ${SOURCES})
target_link_libraries (objectTracking ${raspicam_CV_LIBS})
ELSE()
MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()
#####################################
