cmake_minimum_required(VERSION 3.20)
project(stb)
add_library(stb STATIC src/stb_image.c)
target_include_directories(stb PUBLIC include)
