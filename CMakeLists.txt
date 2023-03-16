cmake_minimum_required(VERSION 3.2)

project(ray)

# Create a debug build (add -fopenmp for faster renderings)
set(CMAKE_CXX_FLAGS "-Wall --std=c++14 -g")

# Set all CPP files to be source files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
# To ensure QtCreator shows the header files, these are also added here
# However, if you do not use QtCreator, feel free to remove the HEADER_FILES
# from the executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
