cmake_minimum_required(VERSION 3.22)
project(mp3_koopas_tycoon_town C)

set(CMAKE_C_STANDARD 99)

add_library(${PROJECT_NAME} hotel.c init.c)
target_include_directories(${PROJECT_NAME} PRIVATE include)