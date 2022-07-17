#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <iostream>

#include "Math.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Manager.hpp"

#include "Renderer2D.hpp"
#include "Camera2D.hpp"

void Main3D() {
    vec2i WinRes(1280, 720);
    std::unique_ptr<Vision::Renderer> renderer = std::make_unique<Vision::Renderer>(WinRes, "VisionGL");
    renderer->InitEnable();
    renderer->Init();

    Vision::Manager::LoadShader("res/shaders/Basic.vert", "res/shaders/Basic.frag", nullptr, "Basic");
    Vision::Manager::LoadTexture("res/textures/Grass_texture.png", true, "Grass");
    Vision::Manager::GetShader("Basic").SetIntArrayInit("u_Textures");

    Vision::Camera camera(WinRes, {0.1f, 500.0f}, 45.0f, 3.0f);

    while (!renderer->WindowShouldClose())
    {
        renderer->Clear({0.53f, 0.81f, 0.94f});
        glm::mat4 mvp = camera.getProjMat() * camera.getViewMat() * glm::mat4(1.0f);
        
        camera.Controls(*renderer);
        Vision::Manager::GetShader("Basic").SetMat4f("u_MVP", mvp);

        renderer->StartBatch();

        renderer->DrawQuad({0, 0, 0.5f} , Vision::CubeOri::Front,   "Grass");
        renderer->DrawQuad({0, -0.5, 0} , Vision::CubeOri::Bottom,  "Grass");
        renderer->DrawQuad({-0.5f, 0, 0}, Vision::CubeOri::Left,   "Grass");
        renderer->DrawQuad({0.5f, 0, 0} , Vision::CubeOri::Right,   "Grass");
        renderer->DrawQuad({0, 0, -0.5f}, Vision::CubeOri::Back,   "Grass");
        renderer->DrawQuad({0, 0.5f, 0} , Vision::CubeOri::Top,     "Grass");
        
        renderer->EndBatch();


        renderer->Flush();
        
        renderer->EndEvents();
    }
    
    Vision::Manager::Clear();
    renderer->Shutdown();
}

void Main2D() {
    vec2i WinRes(1280, 720);
    std::unique_ptr<Vision2D::Renderer2D> renderer = std::make_unique<Vision2D::Renderer2D>(WinRes, "VisionGL");
    renderer->InitEnable();
    

    renderer->Init();
    Vision::Manager::LoadShader("res/shaders/Basic2D.vert", "res/shaders/Basic2D.frag", nullptr, "Basic2D");

    Vision::Manager::LoadTexture("res/textures/Grass_texture.png", true, "Grass");
    Vision::Manager::GetShader("Basic2D").SetIntArrayInit("u_Textures");

    Vision2D::Camera2D camera(WinRes, 1.0f, 12.0f);


    while (!renderer->WindowShouldClose())
    {
        renderer->Clear({0.53f, 0.81f, 0.94f});
        glm::mat4 mvp = camera.getProjMat() * camera.getViewMat() * glm::mat4(1.0f);
        
        camera.Controls(*renderer);
        Vision::Manager::GetShader("Basic2D").SetMat4f("u_MVP", mvp, true);

        renderer->StartBatch();

        for (int i = -5; i < 5; i++)
        {
            for (int j = -5; j < 5; j++)
            {
                renderer->DrawQuad({i, j}, {1,1}, "Grass");
            }
            
        }
        
        
        renderer->EndBatch();


        renderer->Flush();
        
        renderer->EndEvents();
    }
    
    Vision::Manager::Clear();
    renderer->Shutdown();
}

int main(int argc, char const *argv[])
{
    //Main3D();
    //Main2D();
    return 0;
}
