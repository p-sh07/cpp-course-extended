cmake_minimum_required(VERSION 3.11)

project(ImgLib CXX)
set(CMAKE_CXX_STANDARD 17)

set(IMGLIB_MAIN_FILES img_lib.h img_lib.cpp)
set(IMGLIB_FORMAT_FILES ppm_image.h ppm_image.cpp)

add_library(ImgLib STATIC ${IMGLIB_MAIN_FILES} ${IMGLIB_FORMAT_FILES})