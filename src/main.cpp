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
    Vision::Renderer::InitEnable(WinRes, "VisionGL");
    Vision::Renderer::Init();

    Vision::Manager::LoadShader("res/shaders/Basic.vert", "res/shaders/Basic.frag", nullptr, "Basic");
    Vision::Manager::LoadTexture("res/textures/Grass_texture.png", true, "Grass");
    Vision::Manager::GetShader("Basic").SetIntArrayInit("u_Textures");

    Vision::Camera camera(WinRes, {0.1f, 500.0f}, 45.0f, 3.0f);

    while (!Vision::Renderer::WindowShouldClose())
    {
        Vision::Renderer::Clear({0.53f, 0.81f, 0.94f});
        glm::mat4 mvp = camera.getProjMat() * camera.getViewMat() * glm::mat4(1.0f);
        
        camera.Controls();
        Vision::Manager::GetShader("Basic").SetMat4f("u_MVP", mvp);

        Vision::Renderer::StartBatch();

        Vision::Renderer::DrawQuad({0, 0, 0.5f} , Vision::CubeOri::Front,   "Grass");
        Vision::Renderer::DrawQuad({0, -0.5, 0} , Vision::CubeOri::Bottom,  "Grass");
        Vision::Renderer::DrawQuad({-0.5f, 0, 0}, Vision::CubeOri::Left,   "Grass");
        Vision::Renderer::DrawQuad({0.5f, 0, 0} , Vision::CubeOri::Right,   "Grass");
        Vision::Renderer::DrawQuad({0, 0, -0.5f}, Vision::CubeOri::Back,   "Grass");
        Vision::Renderer::DrawQuad({0, 0.5f, 0} , Vision::CubeOri::Top,     "Grass");
        
        Vision::Renderer::EndBatch();


        Vision::Renderer::Flush();
        
        Vision::Renderer::EndEvents();
    }
    
    Vision::Manager::Clear();
    Vision::Renderer::Shutdown();
}

void Main2D() {
    vec2i WinRes(1280, 720);
    Vision2D::Renderer2D::InitEnable(WinRes, "VisionGL");
    

    Vision2D::Renderer2D::Init();
    Vision::Manager::LoadShader("res/shaders/Basic2D.vert", "res/shaders/Basic2D.frag", nullptr, "Basic2D");

    Vision::Manager::LoadTexture("res/textures/Grass_texture.png", true, "Grass");
    Vision::Manager::GetShader("Basic2D").SetIntArrayInit("u_Textures");

    Vision2D::Camera2D camera(WinRes, 1.0f, 12.0f);


    while (!Vision2D::Renderer2D::WindowShouldClose())
    {
        Vision2D::Renderer2D::Clear({0.53f, 0.81f, 0.94f});
        glm::mat4 mvp = camera.getProjMat() * camera.getViewMat() * glm::mat4(1.0f);
        
        camera.Controls();
        Vision::Manager::GetShader("Basic2D").SetMat4f("u_MVP", mvp, true);

        Vision2D::Renderer2D::StartBatch();

        float rot = glfwGetTime()*75;

        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                Vision2D::Renderer2D::DrawQuad({i*32+90, j*32+90}, {32,32}, rot, "Grass");
            }
            
        }

        Vision2D::Renderer2D::EndBatch();

        Vision2D::Renderer2D::Flush();
        
        Vision2D::Renderer2D::EndEvents();
    }
    
    Vision::Manager::Clear();
    Vision2D::Renderer2D::Shutdown();
}

int main(int argc, char const *argv[])
{
    //Main3D();
    //Main2D();
    return 0;
}
