####################################
cmake_minimum_required (VERSION 2.8) 
project (raspicam_test)
set(raspicam_DIR "~/Documents/cpp/rpicam/raspicam-0.1.1/build")
find_package(raspicam REQUIRED)
find_package(OpenCV)
IF  ( OpenCV_FOUND AND raspicam_CV_FOUND)
MESSAGE(STATUS "COMPILING OPENCV TESTS")
add_executable (objectTracking main.cpp)  
target_link_libraries (objectTracking ${raspicam_CV_LIBS})
ELSE()
MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()
#####################################
