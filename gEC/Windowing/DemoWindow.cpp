//
// Created by scion on 11/14/2022.
//

#include <iostream>
#include "DemoWindow.h"
#include "GLAD/glad.h"
#include "gEModel.h"
#include "../Utility/PVR.h"
#include "../Asset/VAO/IndexedVAO.h"
#include "GLFW/glfw3.h"
#include "../Component/Components/PerspectiveCamera.h"
#include "../Component/Components/MaterialHolder.h"
#include "../../res/shader/PBRMaterial.h"

#define LOG(msg) std::cout << "LOG: " << msg << std::endl

gE::DemoWindow::DemoWindow(const char* const title, const uint32_t width, const uint32_t height, gE::Result* const result)
        : Window(title, width, height, result), TransformManager(), AssetManager(), BehaviorManager()
{ }

void APIENTRY DebugCallback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei len, const GLchar* msg, const void* usrPrm)
{
    std::cout << msg << std::endl;
}

void gE::DemoWindow::Load()
{
    glfwSetInputMode(GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(DebugCallback, nullptr);

    auto* dShader = new Asset::Shader(this, "../res/shader/default.vert", "../res/shader/default.frag");
    AssetManager.Add(dShader);
    AssetManager.Add(Skybox.SkyboxShader = new Asset::Shader(this, "../res/shader/skybox.vert", "../res/shader/skybox.frag", Asset::CullMode::NEVER, Asset::DepthFunction::LEQUAL));
    AssetManager.Add(Skybox.SkyboxVAO = gE::Utility::CreateSkyboxVAO(this));

    Asset::Texture* tex;
    AssetManager.Add(tex = Utility::LoadPVR(this, "../x.pvr", nullptr));
    AssetManager.Add(Skybox.SkyboxTexture = Utility::LoadPVR(this, "../sky.pvr", nullptr));

    uint64_t handle = tex->GetHandle();
    glProgramUniform2uiv(dShader->Get(), glGetUniformLocation(dShader->Get(), "Albedo"), 1, (GLuint*) &handle);
    handle = Skybox.SkyboxTexture->GetHandle();
    glProgramUniform2uiv(Skybox.SkyboxShader->Get(), glGetUniformLocation(Skybox.SkyboxShader->Get(), "Skybox"), 1, (GLuint*) &handle);

    uint32_t meshCount;
    gE::Mesh* meshes = gE::LoadgEMeshFromIntermediate("../cube.dae", &meshCount);
    auto* rMesh = new Asset::RenderMesh(this, meshes);

    auto* entity = EntityManager.Create<Entity>(this);
    entity->AddComponent(TransformManager.Create<Component::Transform>(entity));
    entity->AddComponent(ComponentManager.Create<Component::Renderer>(entity, rMesh));

    Asset::Material* entityMaterials = new Asset::PBRMaterial(this, dShader);
    entity->AddComponent(ComponentManager.Create<Component::MaterialHolder>(entity, &entityMaterials, 1));

    entity = EntityManager.Create<Entity>(this);
    entity->AddComponent(TransformManager.Create<Component::Transform>(entity));
    entity->GetComponent<Component::Transform>()->Location.x = 5;
    entity->AddComponent(ComponentManager.Create<Component::Renderer>(entity, rMesh));
    entity->AddComponent(ComponentManager.Create<Component::MaterialHolder>(entity, &entityMaterials, 1));

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
    CameraManager->OnUpdate(0);
    MeshManager->OnUpdate(0);
}

void gE::DemoWindow::Render(double delta)
{
    TransformManager.OnRender(0);
    CameraManager->OnRender(0);

    Stage = Windowing::Stage::PreZ;

    glColorMask(false, false, false, false);
    glDepthMask(true);
    glClear(GL_DEPTH_BUFFER_BIT);

    MeshManager->OnRender();

    Stage = Windowing::Stage::Render;

    glColorMask(true, true, true, true);
    glDepthMask(false);
    glDepthFunc(GL_EQUAL);

    MeshManager->OnRender();
    Skybox.Render();
}

