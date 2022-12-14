//
// Created by scion on 11/2/2022.
//

#include "Window.h"
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

namespace gE
{
    Window::Window(const char* const title, const uint32_t width, const uint32_t height, Result* const result)
            : Title(title), Size(width, height), IncludeManager(), ComponentManager()
    {
        GLFWWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

        if (!GLFWWindow)
        {
            *result = FAIL;
            return;
        }

        glfwMakeContextCurrent(GLFWWindow);

        *result = !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) ? FAIL : OK;

        CameraManager = new Component::CameraManager(this);
        MeshManager = new Asset::MeshManager(this);
        p_MissingShader = new Asset::Shader(this, "../gEC/Resource/depth.vert", "../gEC/Resource/empty.frag", Asset::CullMode::BACKFACE);
    }

    Window::~Window()
    {
        delete p_MissingShader;
        glfwDestroyWindow(GLFWWindow);
        GLFWWindow = nullptr;
    }

    void Window::Run()
    {
        Load();

        double pTime = glfwGetTime();
        while (!glfwWindowShouldClose(GLFWWindow))
        {
            glfwPollEvents();

            Update(glfwGetTime() - pTime); // TODO: Multithread
            double nTime = glfwGetTime();
            Render(nTime - pTime);
            pTime = nTime;

            glfwSwapBuffers(GLFWWindow);
        }
    }

    Result Init(int32_t major, int32_t minor, bool debug)
    {
        if (!glfwInit()) return FAIL;

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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