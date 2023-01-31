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
#include <glm/trigonometric.hpp>
#include "../Asset/Buffer/Renderbuffer.h"
#include "../Asset/Texture/Texture2D.h"
#include "../../res/script/StaticRenderer.h"
#include "../../res/script/InventoryScript.h"

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
#define CEIL_DIV(x, y) ((((x) + (y) - 1) / (y)) ?: 1)
#define HIZ_WORK_GROUP_SIZE 32

gE::DemoWindow::DemoWindow(const char* const title, const uint32_t width, const uint32_t height, gE::Result* const result)
        : Window(title, width, height, result), LightManager(this)
{ }

void APIENTRY DebugCallback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei len, const GLchar* msg, const void* usrPrm)
{
    std::cout << msg << std::endl;
}

void gE::DemoWindow::Load()
{
    // Engine Setup
    glfwSetInputMode(GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    //glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    //glDebugMessageCallback(DebugCallback, nullptr);

    DemoUniformBuffer = AssetManager.Create<Buffer<DemoUBO>>();
    DemoUniformBuffer->Bind(2, BufferTarget::UNIFORM);

    PassthroughVAO = AssetManager.Create<Asset::VAO>(gE::FieldInfo(false, false, false, false), 6, (void*) &PassthroughVertices);
    PassthroughShader = AssetManager.Create<Asset::Shader>("../gEC/Resource/passthrough.vert", "../gEC/Resource/passthrough.frag", Asset::CullMode::NEVER, Asset::DepthFunction::ALWAYS);
    HiZComputeShader = AssetManager.Create<Asset::Shader>("../res/shader/highz.comp");
    Skybox.SkyboxShader = AssetManager.Create<Asset::Shader>("../res/shader/skybox.vert", "../res/shader/skybox.frag", Asset::CullMode::NEVER, Asset::DepthFunction::LEQUAL);

    AssetManager.Add(Skybox.SkyboxVAO = gE::Utility::CreateSkyboxVAO(this));
    Skybox.SkyboxTexture = (Asset::Texture*) AssetManager.Add(Utility::LoadPVR(this, "../sky.pvr", nullptr));

    DepthTex = AssetManager.Create<Asset::Texture2D>(GetSize().x, GetSize().y, Asset::TextureType::DEPTH_32F, 1, Asset::TextureFilterMode::NEAREST, Asset::TextureWrapMode::EDGE);
    PrevDepthTex = AssetManager.Create<Asset::Texture2D>(GetSize().x, GetSize().y, Asset::TextureType::RED_32F, 0, Asset::TextureFilterMode::NEAREST, Asset::TextureWrapMode::BORDER);
    FrameTex = AssetManager.Create<Asset::Texture2D>(GetSize().x, GetSize().y, Asset::TextureType::RGBAf_32, 1, Asset::TextureFilterMode::LINEAR, Asset::TextureWrapMode::EDGE);
    PrevFrameTex = AssetManager.Create<Asset::Texture2D>(GetSize().x, GetSize().y, Asset::TextureType::RGBAf_32, 1, Asset::TextureFilterMode::LINEAR, Asset::TextureWrapMode::EDGE);

    RenderFrameBuffer = AssetManager.Create<Asset::Framebuffer>();
    RenderFrameBuffer->Attach(DepthTex, Asset::Framebuffer::DEPTH);
    RenderFrameBuffer->Attach(FrameTex, 0);

    BlitBuffer = AssetManager.Create<Asset::Framebuffer>();
    BlitBuffer->Attach(AssetManager.Create<Asset::Renderbuffer>(GetSize().x, GetSize().y, Asset::TextureType::DEPTH_32F), Asset::Framebuffer::DEPTH);
    BlitBuffer->Attach(PrevDepthTex, 0);

    // Scene Setup
    
    auto* shinyShader = AssetManager.Create<Asset::Shader>("../res/shader/default.vert", "../res/shader/default.frag");
    //auto* ssrShader = AssetManager.Create<Asset::Shader>("../res/shader/default.vert", "../res/shader/ssr.frag");
    auto* sssShader = AssetManager.Create<Asset::Shader>("../res/shader/default.vert", "../res/shader/contactshadow.frag");
    auto* rMesh = AssetManager.Create<Asset::RenderMesh>(gE::LoadgEMeshFromIntermediate("../cube.dae"));
    //auto* rMeshPlane = AssetManager.Create<Asset::RenderMesh>(gE::LoadgEMeshFromIntermediate("../plane.dae"));

    Asset::Material* shinyMat = AssetManager.Create<Asset::PBRMaterial>(shinyShader);
    //Asset::Material* ssrMat = AssetManager.Create<Asset::PBRMaterial>(ssrShader);
    Asset::Material* sssMat = AssetManager.Create<Asset::PBRMaterial>(sssShader);

    Asset::Material* mats[2]{ shinyMat, shinyMat};

    uint64_t handle = ((Asset::Texture*) AssetManager.Add(Utility::LoadPVR(this, "../x.pvr", nullptr)))->GetHandle();
    glProgramUniform2uiv(shinyShader->Get(), glGetUniformLocation(shinyShader->Get(), "Albedo"), 1, (GLuint*) &handle);
    glProgramUniform2uiv(sssShader->Get(), glGetUniformLocation(sssShader->Get(), "Albedo"), 1, (GLuint*) &handle);

    EntityManager.Create<StaticRenderer>(Transform(glm::vec3(0), glm::vec3(0, 0, 0), glm::vec3(3)), rMesh, mats, 2);
    //EntityManager.Create<StaticRenderer>(Transform(glm::vec3(0), glm::vec3(-90, 0, 0), glm::vec3(3)), rMeshPlane, &ssrMat, 1);

    auto* entity = EntityManager.Create<DynamicEntity>();
    entity->CreateComponent<Component::Transform>(TransformManager);
    entity->CreateComponent<Component::PerspectiveCamera>(CameraManager, 80, glm::vec2(0.1, 1000))->Use();
    entity->CreateComponent<Component::CameraMovement>(&BehaviorManager);

    entity = EntityManager.Create<DynamicEntity>(entity);
    entity->CreateComponent<Component::Transform>(TransformManager);
    entity->CreateComponent<Component::Renderer>(&ComponentManager, nullptr);
    entity->CreateComponent<Component::MaterialHolder>(&ComponentManager, &sssMat, 1);

    auto* gunGETF = LoadgEMeshFromIntermediate("../gun.dae");
    entity->CreateComponent<Component::InventoryScript>(&BehaviorManager)->Weapon = new Weapon
    (
        0.2, FireSelectMode::Single,
        Transform(glm::vec3(1.82343, -1.10008, -1.45051), glm::vec3(4, 0, 5.72), glm::vec3(1)),
        Transform(glm::vec3(0, 0.332703, -0.0976), glm::vec3(0), glm::vec3(1)), // Sight point
            new SightAttachment
            (
                AssetManager.Create<Asset::RenderMesh>(gunGETF), nullptr,
                Transform(glm::vec3(0, 0.12513, -1), glm::vec3(0), glm::vec3(1)),
                80
            ),
        AssetManager.Create<Asset::RenderMesh>(gunGETF + 1),
        nullptr, 0
    );

    entity = EntityManager.Create<DynamicEntity>(entity, "Sight");
    entity->CreateComponent<Component::Transform>(TransformManager);
    entity->CreateComponent<Component::Renderer>(&ComponentManager, nullptr);
    entity->CreateComponent<Component::MaterialHolder>(&ComponentManager, &sssMat, 1);

    entity = EntityManager.Create<DynamicEntity>();
    entity->CreateComponent<Component::Transform>(TransformManager, Transform(glm::vec3(0, 10, 0), glm::vec3(-70, 45, 0), glm::vec3(1)));
    Sun = entity->CreateComponent<Component::DirectionalLight>(&LightManager, 1024, 10);
}

void gE::DemoWindow::Update(double delta)
{
    BehaviorManager.OnUpdate(delta);
    EntityManager.OnUpdate(0);
    LightManager.OnUpdate(0);
    TransformManager->OnRender(0);
}

void gE::DemoWindow::Render(double delta)
{
    Stage = Windowing::Stage::Shadow;

    Component::Camera* prevCam = CameraManager->GetCamera();
    LightManager.OnRender(0);
    {
        DemoUBO buf(Skybox.SkyboxTexture, Sun, PrevFrameTex, DepthTex, Frame);
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

    Stage = Windowing::Stage::PostProcess;
    BlitBuffer->Bind();
    PassthroughShader->Use();
    glProgramUniform1i(PassthroughShader->Get(), 0, DepthTex->Use(1));
    PassthroughVAO->Draw(1);

    HiZComputeShader->Use();
    for(uint8_t i = 1; i < PrevDepthTex->GetMipCount(); i++)
    {
        auto mipSize = PrevDepthTex->GetSize(i - 1);

        PrevDepthTex->Bind(0, Asset::AccessMode::READ, i - 1);
        PrevDepthTex->Bind(1, Asset::AccessMode::WRITE, i);

        glDispatchCompute(CEIL_DIV(mipSize.x, HIZ_WORK_GROUP_SIZE), CEIL_DIV(mipSize.y, HIZ_WORK_GROUP_SIZE), 1);
    }

    Stage = Windowing::Stage::Render;
    RenderFrameBuffer->Bind();
    glColorMask(true, true, true, true);
    glDepthMask(false);
    glDepthFunc(GL_EQUAL);

    MeshManager->OnRender();
    Skybox.Render();

    Stage = Windowing::Stage::PostProcess;

    glCopyImageSubData(FrameTex->Get(), GL_TEXTURE_2D, 0, 0, 0, 0, PrevFrameTex->Get(), GL_TEXTURE_2D, 0, 0, 0, 0, GetSize().x, GetSize().y, 1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    PassthroughShader->Use();
    glProgramUniform1i(PassthroughShader->Get(), 0, FrameTex->Use(1));
    PassthroughVAO->Draw(1);
}

gE::DemoUBO::DemoUBO(gE::Asset::Texture* sky, gE::Component::DirectionalLight* sun, gE::Asset::Texture* prevFrame, gE::Asset::Texture* depthTex, int32_t frame) :
                    ShadowID(sun->GetShadowMap()->GetHandle()), SkyboxID(sky->GetHandle()),
                    ColorID(prevFrame->GetHandle()), DepthID(depthTex->GetHandle()),
                    SunMatrix(*sun->GetProjection() * sun->GetView()), Frame(frame)
{
    Component::Transform* transform = sun->GetOwner()->GetComponent<Component::Transform>();

    glm::vec3 sunDir;
    sunDir.x =  cos(glm::radians(transform->Rotation.x)) * sin(glm::radians(transform->Rotation.y));
    sunDir.y = -sin(glm::radians(transform->Rotation.x));
    sunDir.z =  cos(glm::radians(transform->Rotation.x)) * cos(glm::radians(transform->Rotation.y));
    sunDir = glm::normalize(sunDir);

    SunInfo = glm::vec4(sunDir, sun->GetSize());
}

