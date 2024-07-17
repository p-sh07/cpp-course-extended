# напишите CMakeLists.txt для сборки трёх программ
# он должен иметь три цели: hmirror, vmirror и sobel

cmake_minimum_required(VERSION 3.11)

project(NegateCXX)
set(CMAKE_CXX_STANDARD 20)

# воспользуйтесь этой переменной при создании цели
set(IMGLIB_FILES img_lib.h img_lib.cpp
            ppm_image.h ppm_image.cpp)

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

#target_link_libraries(negate ${SYSTEM_LIBS})

add_executable(hmirror hmirr.cpp ${IMGLIB_FILES})
add_executable(vmirror vmirr.cpp ${IMGLIB_FILES})
add_executable(sobel sobel.cpp ${IMGLIB_FILES})

#cmake ../negate_cmake
#cmake --build . 
