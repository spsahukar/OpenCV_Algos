cmake_minimum_required(VERSION 2.8)

#SET(OpenCV_INSTALL_PATH opt/intel/openvino/opencv/)
#SET(OpenCV_INCLUDE_DIRS "${OpenCV_INSTALL_PATH}/include/opencv;${OpenCV_INSTALL_PATH}/include")
message("OpenCV_FOUND='${OpenCV_FOUND}'")
#SET(OpenCV_LIB_DIR "${OpenCV_INSTALL_PATH}/lib")

#LINK_DIRECTORIES(${OpenCV_LIB_DIR})

#set(OpenCV_LIBS  opencv_imgproc opencv_video opencv_features2d opencv_ml opencv_highgui opencv_objdetect opencv_contrib opencv_legacy opencv_gpu)

find_package(OpenCV REQUIRED)

message("OpenCV_FOUND='${OpenCV_FOUND}'")
project(Debayer)

include_directories(
        ${Debayer_SOURCE_DIR}/include
)
message("These are my opencv libs....: '${OpenCV_LIBS}'" )
add_executable(debayer ${Debayer_SOURCE_DIR}/src/main.cpp ${Debayer_SOURCE_DIR}/include/debayer.hpp)
TARGET_LINK_LIBRARIES(debayer ${OpenCV_LIBS} )
