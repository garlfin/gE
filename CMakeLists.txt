cmake_minimum_required(VERSION 3.22.1)
project(gEP)

### SETUP ###

set(CMAKE_CXX_STANDARD 23)
file(GLOB_RECURSE gECFiles  "gEC/*.cpp"
        "gEC/*.c")
file(GLOB_RECURSE gEGLFiles "gEGL/*.cpp"
                            "gEGL/*.c")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

include_directories(gEGL gEModel)

# gEGL
add_library(gEGL SHARED ${gEGLFiles})
find_library(glfw glfw3 gEGL/GLFW/)
target_link_libraries(gEGL ${glfw})

# gE
add_library(gEModel SHARED gEModel/gEModel.cpp)
target_link_libraries(gEModel ${CMAKE_CURRENT_SOURCE_DIR}/libassimp-5.dll)

# gEC
add_executable(gEC ${gECFiles} gEC/Component/Components/Camera.cpp gEC/Component/Components/Renderer.cpp gEC/Component/Components/Renderer.h gEC/Component/Components/Behavior.h res/script/CameraMovement.cpp res/script/CameraMovement.h gEC/Asset/Buffer/Renderbuffer.cpp gEC/Asset/Buffer/Renderbuffer.h gEC/Asset/Buffer/Framebuffer.cpp gEC/Asset/Buffer/Framebuffer.h gEC/Asset/VAO/MeshManager.cpp gEC/Asset/VAO/MeshManager.h)
target_link_libraries(gEC gEGL gEModel)