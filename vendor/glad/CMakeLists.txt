cmake_minimum_required(VERSION 3.20)
project(glad)
add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)
