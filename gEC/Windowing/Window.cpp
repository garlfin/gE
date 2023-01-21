//
// Created by scion on 11/2/2022.
//

#include "Window.h"
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

#define FPS_TARGET 144

namespace gE
{
    Window::Window(const char* const title, const uint32_t width, const uint32_t height, Result* const result)
            : Title(title), Size(width, height), EntityManager(this), IncludeManager(this), ComponentManager(this), AssetManager(this), BehaviorManager(this)
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
        TransformManager = new Component::TransformManager(this);
        p_MissingShader = new Asset::Shader(this, "../gEC/Resource/depth.vert", "../gEC/Resource/empty.frag", Asset::CullMode::NEVER);

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    Window::~Window()
    {
        delete p_MissingShader;
        delete MeshManager;
        delete CameraManager;
        delete TransformManager;

        glfwDestroyWindow(GLFWWindow);
        GLFWWindow = nullptr;
    }

    void Window::Run()
    {
        Load();

        double pTime = glfwGetTime();
         Frame = 0;

        while (!glfwWindowShouldClose(GLFWWindow))
        {
            glfwPollEvents();

            double nTime = glfwGetTime();
            Update(nTime - pTime); // TODO: Multithread


            if(nTime > Frame * (1.0 / FPS_TARGET))
            {
                nTime = glfwGetTime();
                Render(nTime - pTime);
                Frame++;

                glfwSwapBuffers(GLFWWindow);
            }

            pTime = nTime;
        }
    }

    Result Init(int32_t major, int32_t minor, bool debug)
    {
        if (!glfwInit()) return FAIL;

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, debug);
        glfwWindowHint(GLFW_REFRESH_RATE, 144);

        return OK;
    }

    Result Destroy()
    {
        glfwTerminate();
        return OK;
    }
}