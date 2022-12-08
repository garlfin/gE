//
// Created by scion on 11/14/2022.
//

#include <iostream>
#include "DemoWindow.h"
#include "GLAD/glad.h"
#include "gEModel.h"
#include "../Utility/PVR.h"
#include "../Asset/Shader/Shader.h"
#include "../Asset/VAO/IndexedVAO.h"
#include "GLFW/glfw3.h"
#include "../Component/Components/PerspectiveCamera.h"

gE::DemoWindow::DemoWindow(const char* const title, const uint32_t width, const uint32_t height, gE::Result* const result)
        : Window(title, width, height, result), TransformManager(), MeshRendererManager(), TextureManager(), BehaviorManager()
{}

void APIENTRY DebugCallback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei len, const GLchar* msg, const void* usrPrm)
{
    std::cout << msg << std::endl;
}

gE::Asset::Shader* DShader;

void gE::DemoWindow::Load()
{
    glfwSetInputMode(GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glClearColor(0, 0, 0, 1);
    // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(DebugCallback, nullptr);

    DShader = new Asset::Shader(this, "../res/shader/default.vert", "../res/shader/default.frag");

    TextureManager.Add(Utility::LoadPVR(this, "../x.pvr"));

    uint32_t meshCount;
    gE::Mesh* meshes = gE::LoadgEMeshFromIntermediate("../cube.dae", &meshCount);
    auto* mesh = new gE::Asset::IndexedVAO(this, meshes->SubMeshes);
    delete[] meshes;

    auto* entity = EntityManager.Create<Entity>(this);
    entity->AddComponent(TransformManager.Create<Component::Transform>(entity));
    entity->AddComponent(MeshRendererManager.Create<Component::Renderer>(entity, mesh));

    auto* camera = EntityManager.Create<Entity>(this);
    camera->AddComponent(TransformManager.Create<Component::Transform>(entity));
    camera->AddComponent(CameraManager->Create<Component::PerspectiveCamera>(camera, 80, glm::vec2(0.1, 1000)));
    camera->GetComponent<Component::PerspectiveCamera>()->Use();
    camera->AddComponent(BehaviorManager.Create<Component::CameraMovement>(camera));
}

void gE::DemoWindow::Update(double delta)
{
    BehaviorManager.OnUpdate(delta);

    EntityManager.OnUpdate(0);
    MeshRendererManager.OnUpdate(0);
    CameraManager->OnUpdate(0);
    TransformManager.OnUpdate(0);
}

void gE::DemoWindow::Render(double delta)
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    DShader->Use();
    TransformManager.OnRender(0);
    CameraManager->OnRender(0);

    MeshRendererManager.OnRender(0);
}

