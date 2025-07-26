cmake_minimum_required(VERSION 3.6)

project(odeer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_compile_options(-fPIC -g)
add_subdirectory(vendor/)

file(GLOB_RECURSE ODEER_SRC CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE ODEER_INC CONFIGURE_DEPENDS include/*.h)

add_executable(${PROJECT_NAME} ${ODEER_SRC} ${ODEER_INC})


target_include_directories(${PROJECT_NAME} PUBLIC src/)
target_include_directories(${PROJECT_NAME} PRIVATE ${ODEER_VENDORED_INCLUDE})
target_link_libraries(${PROJECT_NAME} ${ODEER_VENDORED_LIBS})
