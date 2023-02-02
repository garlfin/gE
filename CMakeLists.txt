cmake_minimum_required(VERSION 3.22.1)
project(gEP)

### SETUP ###
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

file(GLOB_RECURSE gECFiles  "gEC/*.cpp"
                             "gEC/*.c")
file(GLOB_RECURSE gEGLFiles "gEGL/*.cpp"
                            "gEGL/*.c")

FILE(COPY gEGL/GLFW/glfw3.dll DESTINATION ${PROJECT_BINARY_DIR})

include_directories(gEGL gEModel)
# gEGL
link_directories("C:/Program Files/JetBrains/CLion 2022.3.1/bin/mingw/lib/gcc/x86_64-w64-mingw32/11.2.0")
add_library(gEGL SHARED ${gEGLFiles})
find_library(glfw glfw3 gEGL/GLFW)
target_link_libraries(gEGL ${glfw})

# gE
add_library(gEModel SHARED gEModel/gEModel.cpp)
target_link_libraries(gEModel ${CMAKE_CURRENT_SOURCE_DIR}/libassimp-5.dll)

# gEC
add_executable(gEC ${gECFiles} res/shader/PBRMaterial.cpp res/script/StaticRenderer.cpp res/script/InventoryScript.cpp res/script/CameraMovement.cpp)
target_link_libraries(gEC gEGL gEModel)