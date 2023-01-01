//
// Created by scion on 11/14/2022.
//

#include <iostream>
#include "DemoWindow.h"
#include "GLAD/glad.h"
#include "gEModel.h"
#include "../Utility/PVR.h"
#include "GLFW/glfw3.h"
#include "../Component/Components/Camera/PerspectiveCamera.h"
#include "../Component/Components/MaterialHolder.h"
#include "../../res/shader/PBRMaterial.h"
#include "glm/trigonometric.hpp"
#include "../Asset/Texture/Texture2D.h"


#define DEPTH_CREATE AssetManager.Create<Asset::Texture2D>(this, this->GetSize().x, this->GetSize().y, Asset::TextureType::DEPTH_32f, 1, gE::Asset::TextureFilterMode::NEAREST)
#define COLOR_CREATE AssetManager.Create<Asset::Texture2D>(this, this->GetSize().x, this->GetSize().y, Asset::TextureType::RGBAf_32)

const float PassthroughVertices[]
        {
            -1, -1, 0,
            -1, 1, 0,
            1, -1, 0,
            1, 1, 0,
            -1, 1, 0,
            1, -1, 0
        };

#define LOG(msg) std::cout << "LOG: " << msg << std::endl

#define CREATE_TRANSFORM(entity) entity->AddComponent(TransformManager->Create<Component::Transform>(entity))

gE::DemoWindow::DemoWindow(const char* const title, const uint32_t width, const uint32_t height, gE::Result* const result)
        : Window(title, width, height, result), LightManager()
{ }

void APIENTRY DebugCallback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei len, const GLchar* msg, const void* usrPrm)
{
    std::cout << msg << std::endl;
}

void gE::DemoWindow::Load()
{
    glfwSetInputMode(GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    DemoUniformBuffer = new Buffer<DemoUBO>(this);
    DemoUniformBuffer->Bind(2, BufferTarget::UNIFORM);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(DebugCallback, nullptr);

    PassthroughVAO = AssetManager.Create<Asset::VAO>(this, gE::FieldInfo(false, false, false, false), 6, (void*) &PassthroughVertices);
    PassthroughShader = AssetManager.Create<Asset::Shader>(this, "../gEC/Resource/passthrough.vert", "../gEC/Resource/passthrough.frag", Asset::CullMode::NEVER, Asset::DepthFunction::ALWAYS);

    auto* shinyShader = AssetManager.Create<Asset::Shader>(this, "../res/shader/default.vert", "../res/shader/default.frag");
    auto* ssrShader = AssetManager.Create<Asset::Shader>(this, "../res/shader/default.vert", "../res/shader/ssr.frag");
    Skybox.SkyboxShader = AssetManager.Create<Asset::Shader>(this, "../res/shader/skybox.vert", "../res/shader/skybox.frag", Asset::CullMode::NEVER, Asset::DepthFunction::LEQUAL);
    AssetManager.Add(Skybox.SkyboxVAO = gE::Utility::CreateSkyboxVAO(this));

    Asset::Texture* tex;
    AssetManager.Add(tex = Utility::LoadPVR(this, "../x.pvr", nullptr));
    AssetManager.Add(Skybox.SkyboxTexture = Utility::LoadPVR(this, "../sky.pvr", nullptr));

    DepthTex = DEPTH_CREATE;
    PrevDepthTex = DEPTH_CREATE;
    FrameTex = COLOR_CREATE;
    PrevFrameTex = COLOR_CREATE;

    RenderFrameBuffer = AssetManager.Create<Asset::Framebuffer>(this);
    RenderFrameBuffer->Attach(DepthTex, Asset::Framebuffer::DEPTH);
    RenderFrameBuffer->Attach(FrameTex, 0);

    uint64_t handle = tex->GetHandle();
    glProgramUniform2uiv(shinyShader->Get(), glGetUniformLocation(shinyShader->Get(), "Albedo"), 1, (GLuint*) &handle);

    uint32_t meshCount;
    gE::Mesh* meshes = gE::LoadgEMeshFromIntermediate("../cube.dae", &meshCount);
    auto* rMesh = new Asset::RenderMesh(this, meshes);

    meshes = gE::LoadgEMeshFromIntermediate("../plane.dae", &meshCount);
    auto* rMeshCopy = new Asset::RenderMesh(this, meshes); // todo finally implement materials because this is really really stupid

    auto* entity = EntityManager.Create<Entity>(this);
    CREATE_TRANSFORM(entity);
    entity->AddComponent(ComponentManager.Create<Component::Renderer>(entity, rMesh));

    Asset::Material* shinyMat = new Asset::PBRMaterial(this, shinyShader);
    Asset::Material* ssrMat = new Asset::PBRMaterial(this, ssrShader);

    entity->GetComponent<Component::Transform>()->Scale = glm::vec3(2);
    entity->GetComponent<Component::Transform>()->Rotation.x = -90;
    entity->AddComponent(ComponentManager.Create<Component::Renderer>(entity, rMesh));
    entity->AddComponent(ComponentManager.Create<Component::MaterialHolder>(entity, &shinyMat, 1));

    entity = EntityManager.Create<Entity>(this);
    CREATE_TRANSFORM(entity);
    entity->GetComponent<Component::Transform>()->Scale = glm::vec3(2);
    entity->GetComponent<Component::Transform>()->Rotation.x = -90;

    entity->AddComponent(ComponentManager.Create<Component::Renderer>(entity, rMeshCopy));
    entity->AddComponent(ComponentManager.Create<Component::MaterialHolder>(entity, &ssrMat, 1));

    entity = EntityManager.Create<Entity>(this);
    CREATE_TRANSFORM(entity);
    entity->AddComponent(CameraManager->Create<Component::PerspectiveCamera>(entity, 80, glm::vec2(0.1, 1000)));
    entity->GetComponent<Component::PerspectiveCamera>()->Use();
    entity->AddComponent(BehaviorManager.Create<Component::CameraMovement>(entity));

    entity = EntityManager.Create<Entity>(this);
    CREATE_TRANSFORM(entity);
    entity->AddComponent(Sun = LightManager.Create<Component::DirectionalLight>(entity, 1024));

    auto* eT = entity->GetComponent<Component::Transform>();
    eT->Location = glm::vec3(0, 10, 0);
    eT->Rotation = glm::vec3(-80, 0, 0);
}

void gE::DemoWindow::Update(double delta)
{
    BehaviorManager.OnUpdate(delta);
    EntityManager.OnUpdate(0);
    LightManager.OnUpdate(0);
}

void gE::DemoWindow::Render(double delta)
{
    Stage = Windowing::Stage::Shadow;

    TransformManager->OnRender(0);
    Component::Camera* prevCam = CameraManager->GetCamera();
    LightManager.OnRender(0);
    {
        DemoUBO buf(Skybox.SkyboxTexture, Sun, PrevFrameTex, PrevDepthTex);
        DemoUniformBuffer->ReplaceData(&buf);
    }
    prevCam->Use();

    Stage = Windowing::Stage::PreZ;

    CameraManager->OnUpdate(0);
    MeshManager->OnUpdate(0);

    RenderFrameBuffer->Bind();
    glViewport(0, 0, GetSize().x, GetSize().y);
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

    Stage = Windowing::Stage::PostProcess;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    PassthroughShader->Use();

    glCopyImageSubData(FrameTex->Get(), GL_TEXTURE_2D, 0, 0, 0, 0, PrevFrameTex->Get(), GL_TEXTURE_2D, 0, 0, 0, 0, GetSize().x, GetSize().y, 1);
    glCopyImageSubData(DepthTex->Get(), GL_TEXTURE_2D, 0, 0, 0, 0, PrevDepthTex->Get(), GL_TEXTURE_2D, 0, 0, 0, 0, GetSize().x, GetSize().y, 1);

    glProgramUniform1i(PassthroughShader->Get(), 0, FrameTex->Use(1));
    PassthroughVAO->Draw(1);
}

gE::DemoUBO::DemoUBO(gE::Asset::Texture* sky, gE::Component::DirectionalLight* sun, gE::Asset::Texture* prevFrame, gE::Asset::Texture* depthTex) :
                    ShadowID(sun->GetShadowMap()->GetHandle()), SkyboxID(sky->GetHandle()),
                    ColorID(prevFrame->GetHandle()), DepthID(depthTex->GetHandle()),
                    SunMatrix(*sun->GetProjection() * sun->GetView())
{
    Component::Transform* transform = sun->GetOwner()->GetComponent<Component::Transform>();

    glm::vec3 sunDir;
    sunDir.x =  cos(glm::radians(transform->Rotation.x)) * sin(glm::radians(transform->Rotation.y));
    sunDir.y = -sin(glm::radians(transform->Rotation.x));
    sunDir.z =  cos(glm::radians(transform->Rotation.x)) * cos(glm::radians(transform->Rotation.y));
    sunDir = glm::normalize(sunDir);

    SunInfo = glm::vec4(sunDir, sun->GetShadowMap()->GetSize().x);
}
