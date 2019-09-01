cmake_minimum_required(VERSION 3.14)
project(Beat_Box C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/cmpt433/public")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

set(CMAKE_C_STANDARD 99)

#SET(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)


add_executable(Beat_Box
        main.c
        audioMixer_template.c
        music.c
        )


target_link_libraries(Beat_Box ~/cmpt433/public/asound_lib_BBB/libasound.so)


target_link_libraries(Beat_Box pthread)