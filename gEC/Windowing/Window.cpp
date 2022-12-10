//
// Created by scion on 11/2/2022.
//

#include "Window.h"
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

namespace gE
{
    Window::Window(const char* const title, const uint32_t width, const uint32_t height, Result* const result)
            : p_Title(title), p_Size(width, height), IncludeManager(), ComponentManager()
    {
        p_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);

        if (!p_Window)
        {
            *result = FAIL;
            return;
        }

        glfwMakeContextCurrent(p_Window);

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) *result = FAIL;

        *result = OK;

        CameraManager = new Component::CameraManager(this);
        MeshManager = new Asset::MeshManager(this);
    }

    Window::~Window()
    {
        glfwDestroyWindow( p_Window);
        p_Window = nullptr;
    }

    void Window::Run()
    {
        Load();

        glfwSetTime(0);
        while (!glfwWindowShouldClose(p_Window))
        {
            glfwPollEvents();

            Update(glfwGetTime()); // TODO: Multithread
            Render(glfwGetTime());

            glfwSetTime(0);
            glfwSwapBuffers(p_Window);
        }
    }

    Result Init(int32_t major, int32_t minor, bool debug)
    {
        if (!glfwInit()) return FAIL;

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, debug);

        return OK;
    }

    Result Destroy()
    {
        glfwTerminate();
        return OK;
    }
}