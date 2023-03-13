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
#include "../../res/script/StaticRenderer.h"
#include "../../res/script/InventoryScript.h"
#include "../Asset/Texture/Texture2D.h"

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

gE::DemoWindow::DemoWindow(const char* const title, const uint32_t width, const uint32_t height, gE::Result* const result)
        : Window(title, width, height, result), LightManager(this)
{ }

void APIENTRY DebugCallback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei len, const GLchar* msg, const void* usrPrm)
{
    if(severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
    std::cout << msg << std::endl;
}

void gE::DemoWindow::Load()
{
    glfwSetInputMode(GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_DEBUG_OUTPUT);
    //glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glClearColor(0, 0, 0, 0);
    //glDebugMessageCallback(DebugCallback, nullptr);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    CubemapManager = new Component::CubemapManager(this);

    BRDF = AssetManager.Create<Asset::Texture2D>(512, 512, Asset::TextureType::RGf_16, 1, Asset::TextureFilterMode::LINEAR, Asset::TextureWrapMode::EDGE);
    {
        Asset::Shader brdfCompute(this, "../res/shader/brdf.comp");
        brdfCompute.Use();
        BRDF->Bind(0, Asset::AccessMode::WRITE);
        glDispatchCompute(16, 16, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }

    DemoUniformBuffer = AssetManager.Create<Buffer<DemoUBO>>();
    DemoUniformBuffer->Bind(2, BufferTarget::UNIFORM);

    PassthroughVAO = AssetManager.Create<Asset::VAO>(gE::FieldInfo(false, false, false, false), 6, (void*) &PassthroughVertices);
    PassthroughShader = AssetManager.Create<Asset::Shader>("../gEC/Resource/passthrough.vert", "../gEC/Resource/passthrough.frag", Asset::CullMode::NEVER, Asset::DepthFunction::ALWAYS);
    TAAShader = AssetManager.Create<Asset::Shader>("../gEC/Resource/passthrough.vert", "../res/shader/taa.frag", Asset::CullMode::NEVER, Asset::DepthFunction::ALWAYS);
    //HiZComputeShader = AssetManager.Create<Asset::Shader>("../res/shader/highz.comp");

    CubemapManager->UpdateSkybox((Asset::Texture*) AssetManager.Add(Utility::LoadPVR(this, "../sky.pvr", nullptr)));


    BlitBuffer = AssetManager.Create<Asset::Framebuffer>();
    BlitBuffer->Attach(AssetManager.Create<Asset::Renderbuffer>(GetSize().x, GetSize().y, Asset::TextureType::DEPTH_32f), Asset::Framebuffer::DEPTH);

    // Scene Setup
    
    auto* shinyShader = AssetManager.Create<Asset::Shader>("../res/shader/default.vert", "../res/shader/default.frag");//, Asset::CullMode::BACKFACE, Asset::DepthFunction::LESS, Asset::CompileFlags::NONE);
    auto* ssrShader = AssetManager.Create<Asset::Shader>("../res/shader/default.vert", "../res/shader/ssr.frag");// Asset::CullMode::NEVER), Asset::DepthFunction::LESS, Asset::CompileFlags::FORWARD);
    auto* tpcShader = AssetManager.Create<Asset::Shader>("../res/shader/default.vert", "../res/shader/alpha.frag");//, Asset::CullMode::NEVER);
    auto* sssShader = AssetManager.Create<Asset::Shader>("../res/shader/default.vert", "../res/shader/contactshadow.frag");
    auto* rMesh = AssetManager.Create<Asset::RenderMesh>(gE::LoadgEMeshFromIntermediate("../plane.dae"));
    auto* rMesh2 = AssetManager.Create<Asset::RenderMesh>(gE::LoadgEMeshFromIntermediate("../cube.dae"));
    //auto* rMeshPlane = AssetManager.Create<Asset::RenderMesh>(gE::LoadgEMeshFromIntermediate("../plane.dae"));

    Asset::Material* shinyMat = AssetManager.Create<Asset::PBRMaterial>(shinyShader, shinyShader);
    Asset::Material* ssrMat = AssetManager.Create<Asset::PBRMaterial>(ssrShader, ssrShader);
    Asset::Material* ssrMatAlpha = AssetManager.Create<Asset::PBRMaterial>(ssrShader, ssrShader, tpcShader);
    Asset::Material* sssMat = AssetManager.Create<Asset::PBRMaterial>(sssShader, ssrShader);

    Asset::Material* mats[2]{ssrMat, shinyMat};
    Asset::Material* matsPlane[2]{ssrMatAlpha, shinyMat};

    uint64_t handleAlbedo = ((Asset::Texture*) AssetManager.Add(Utility::LoadPVR(this, "../tile.pvr", nullptr)))->GetHandle();
    uint64_t handleRough = ((Asset::Texture*) AssetManager.Add(Utility::LoadPVR(this, "../tile_rough.pvr", nullptr)))->GetHandle();
    uint64_t handleNor = ((Asset::Texture*) AssetManager.Add(Utility::LoadPVR(this, "../tile_nor.pvr", nullptr)))->GetHandle();

    uint64_t gunAlbedo = ((Asset::Texture*) AssetManager.Add(Utility::LoadPVR(this, "../res/m18.pvr", nullptr)))->GetHandle();
    uint64_t gunRough = ((Asset::Texture*) AssetManager.Add(Utility::LoadPVR(this, "../res/m18_rough.pvr", nullptr)))->GetHandle();
    uint64_t gunNor = ((Asset::Texture*) AssetManager.Add(Utility::LoadPVR(this, "../res/m18_nor.pvr", nullptr)))->GetHandle();
    uint64_t chainAlbedo = ((Asset::Texture*) AssetManager.Add(Utility::LoadPVR(this, "../res/chain.pvr", nullptr)))->GetHandle();

    glProgramUniformHandleui64ARB(shinyShader->Get(), glGetUniformLocation(shinyShader->Get(), "Albedo"),    handleAlbedo);
    glProgramUniformHandleui64ARB(shinyShader->Get(), glGetUniformLocation(shinyShader->Get(), "Roughness"), handleRough);
    glProgramUniformHandleui64ARB(shinyShader->Get(), glGetUniformLocation(shinyShader->Get(), "NormalTex"), handleNor);

    glProgramUniformHandleui64ARB(ssrShader->Get(), glGetUniformLocation(ssrShader->Get(), "NormalTex"), handleNor);

    glProgramUniformHandleui64ARB(sssShader->Get(), glGetUniformLocation(sssShader->Get(), "Albedo"), gunAlbedo);
    glProgramUniformHandleui64ARB(sssShader->Get(), glGetUniformLocation(sssShader->Get(), "NormalTex"), gunNor);
    glProgramUniformHandleui64ARB(sssShader->Get(), glGetUniformLocation(sssShader->Get(), "Roughness"), gunRough);

    glProgramUniformHandleui64ARB(tpcShader->Get(), glGetUniformLocation(tpcShader->Get(), "Albedo"), chainAlbedo);

    EntityManager.Create<StaticRenderer>(Transform(glm::vec3(0), glm::vec3(0, 0, 0), glm::vec3(2.4)), rMesh, matsPlane, 2);
    EntityManager.Create<StaticRenderer>(Transform(glm::vec3(0), glm::vec3(0, 0, 0), glm::vec3(2.4)), rMesh2, mats, 2)->Layer = Layers::Two;

    //EntityManager.Create<StaticRenderer>(Transform(glm::vec3(0), glm::vec3(0, 0, 0), glm::vec3(10)), rMesh, mats, 1)->Layer = Layers::Two;
    //EntityManager.Create<StaticRenderer>(Transform(glm::vec3(0), glm::vec3(-90, 0, 0), glm::vec3(3)), rMeshPlane, &ssrMat, 1);

    auto* entity = EntityManager.Create<DynamicEntity>();
    entity->Layer |= Layers::Two;
    entity->CreateComponent<Component::Transform>(TransformManager, Transform(glm::vec3(0, 2, 0), glm::vec3(0), glm::vec3(1)));
    entity->CreateComponent<Component::PerspectiveCamera>(CameraManager, 1, glm::vec2(0.1, 100))->Use();

    auto* fpsCam = EntityManager.Create<DynamicEntity>(entity);
    fpsCam->Layer = Layers::One;
    fpsCam->CreateComponent<Component::Transform>(TransformManager, Transform(glm::vec3(0), glm::vec3(0), glm::vec3(1)));
    fpsCam->CreateComponent<Component::PerspectiveCamera>(CameraManager, 80, glm::vec2(0.1, 100));

    entity->CreateComponent<Component::CameraMovement>(&BehaviorManager, fpsCam);

    entity = EntityManager.Create<DynamicEntity>(entity);
    entity->Layer = Layers::One;
    entity->CreateComponent<Component::Transform>(TransformManager);
    entity->CreateComponent<Component::Renderer>(&ComponentManager, nullptr);
    entity->CreateComponent<Component::MaterialHolder>(&ComponentManager, &sssMat, 1);

    auto* gunGETF = LoadgEMeshFromIntermediate("../gun.dae");
    entity->CreateComponent<Component::InventoryScript>(&BehaviorManager)->Weapon = new Weapon
    (
        0.2, FireSelectMode::Single,
        Transform(glm::vec3(1.82343, -1.10008, -1.45051), glm::vec3(4, 0, 5.72), glm::vec3(1)),
        Transform(glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(1)), // Sight point
            new SightAttachment
            (
                    nullptr, nullptr,
                Transform(glm::vec3(0, 0.382211, -1.23802), glm::vec3(0), glm::vec3(1)),
                60
            ),
        AssetManager.Create<Asset::RenderMesh>(gunGETF),
        nullptr, 0
    );

    entity = EntityManager.Create<DynamicEntity>(entity, "Sight");
    entity->CreateComponent<Component::Transform>(TransformManager);
    entity->CreateComponent<Component::Renderer>(&ComponentManager, nullptr);
    entity->CreateComponent<Component::MaterialHolder>(&ComponentManager, &sssMat, 1);

    entity = EntityManager.Create<DynamicEntity>(nullptr, "Sun");
    entity->Layer = Layers::All;
    entity->CreateComponent<Component::Transform>(TransformManager, Transform(glm::vec3(0), glm::vec3(-145, 65, 0), glm::vec3(1)));
    Sun = entity->CreateComponent<Component::DirectionalLight>(&LightManager, 1024, 30);

    entity = EntityManager.Create<DynamicEntity>();
    entity->Layer = Layers::Default;
    auto* cTransform = entity->CreateComponent<Component::Transform>(TransformManager, Transform(glm::vec3(0, 10, 0), glm::vec3(0), glm::vec3(1)));
    auto* cCam = entity->CreateComponent<Component::CubemapCamera>(CubemapManager, 512, glm::vec2(0.1, 100));


    auto* sunTransform = Sun->GetOwner()->GetComponent<gE::Component::Transform>();
    glm::vec3 sunDir;
    sunDir.x =  cos(glm::radians(sunTransform->Rotation.x)) * sin(glm::radians(sunTransform->Rotation.y));
    sunDir.y = -sin(glm::radians(sunTransform->Rotation.x));
    sunDir.z =  cos(glm::radians(sunTransform->Rotation.x)) * cos(glm::radians(sunTransform->Rotation.y));
    sunTransform->Location = glm::normalize(sunDir) * Sun->GetSize() + cTransform->Location;

    Update(0);
    TransformManager->OnUpdate(0);
    {
        DemoUBO ubo(Sun, BRDF, GetFrame());
        DemoUniformBuffer->ReplaceData(&ubo);
    }

    Sun->OnRender(0);

    Component::CubemapBufferData d{CubemapManager->Skybox.SkyboxTexture->GetHandle(), Component::CubemapData(CubemapManager->Skybox.SkyboxTexture->GetHandle(), cTransform->Location, glm::vec3(10.01))};
    CubemapManager->CubemapBuffer.ReplaceData(&d);

    cCam->OnRender(0);

    d.Cubemaps->CubemapHandle = cCam->GetColor()->GetHandle();
    CubemapManager->CubemapBuffer.ReplaceData(&d);
}

void gE::DemoWindow::Update(double delta)
{
    BehaviorManager.OnUpdate(delta);
    EntityManager.OnUpdate(0);
    LightManager.OnUpdate(0);
}

void gE::DemoWindow::Render(double delta)
{
    TransformManager->OnUpdate(0);

    auto* sunTransform = Sun->GetOwner()->GetComponent<Component::Transform>();

    glm::vec3 sunDir;
    sunDir.x =  cos(glm::radians(sunTransform->Rotation.x)) * sin(glm::radians(sunTransform->Rotation.y));
    sunDir.y = -sin(glm::radians(sunTransform->Rotation.x));
    sunDir.z =  cos(glm::radians(sunTransform->Rotation.x)) * cos(glm::radians(sunTransform->Rotation.y));
    sunTransform->Location = glm::normalize(sunDir) * 25.f;
    sunTransform->Location += glm::floor(CameraManager->GetCamera()->GetOwner()->GetComponent<Component::Transform>()->Location);

    {
        DemoUBO ubo(Sun, BRDF, Frame);
        DemoUniformBuffer->ReplaceData(&ubo);
    }

    Sun->OnRender(delta);
    CameraManager->GetCamera()->OnRender(delta);

    BehaviorManager.OnRender(delta);

    glViewport(0, 0, GetSize().x, GetSize().y);

    Stage = Windowing::Stage::PostProcess;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    PassthroughShader->Use();
    glProgramUniform1i(PassthroughShader->Get(), 0, CameraManager->GetCamera()->GetColor()->Use(0));
    PassthroughVAO->Draw(1);
}

void gE::DemoWindow::SetStage(gE::Windowing::Stage stage, bool replaceBuffer)
{
    Window::SetStage(stage);
    if(!replaceBuffer) return;
    int32_t stg((int32_t) stage);
    DemoUniformBuffer->ReplaceData(&stg, 1, offsetof(DemoUBO, Stage), 1);
}

gE::DemoUBO::DemoUBO(gE::Component::DirectionalLight* sun, gE::Asset::Texture* brdf, int32_t frame) :
                    ShadowID(sun->GetDepth()->GetHandle()), SunMatrix(sun->GetProjection() * sun->GetView()),
                    Frame(frame), BRDFID(brdf->GetHandle()), Stage((int32_t) sun->GetWindow()->GetStage())
{
    Component::Transform* transform = sun->GetOwner()->GetComponent<Component::Transform>();

    glm::vec3 sunDir;
    sunDir.x =  cos(glm::radians(transform->Rotation.x)) * sin(glm::radians(transform->Rotation.y));
    sunDir.y = -sin(glm::radians(transform->Rotation.x));
    sunDir.z =  cos(glm::radians(transform->Rotation.x)) * cos(glm::radians(transform->Rotation.y));
    sunDir = glm::normalize(sunDir);

    SunInfo = glm::vec4(sunDir, sun->GetSize());
}

