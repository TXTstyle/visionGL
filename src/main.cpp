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

int main(int argc, char const *argv[])
{
    vec2i WinRes(1280, 720);
    std::unique_ptr<Vision::Renderer> renderer = std::make_unique<Vision::Renderer>(WinRes, "VisionGL");
    renderer->InitEnable();

    std::unique_ptr<Vision::Shader> shader = std::make_unique<Vision::Shader>("res/shaders/Basic.shader");
    shader->Bind();
    
    renderer->Init();

    Vision::Texture grass("res/textures/Grass_texture.png");
    int samplers[32];
    for (int i = 0; i < 32; i++)
        samplers[i] = i;
    shader->SetUniform1iv("u_Textures", 32, samplers);

    Vision::Camera camera(WinRes, vec2f(0.1f, 500.0f), 45.0f, 3.0f);

    //std::cout << texture->GetID() << std::endl;

    while (!renderer->WindowShouldClose())
    {
        renderer->Clear(vec3f(0.53f, 0.81f, 0.94f));
        glm::mat4 mvp = camera.getProjMat() * camera.getViewMat() * glm::mat4(1.0f);
        
        camera.Controls(*renderer);
        shader->SetUniformMat4f("u_MVP", mvp);

        renderer->StartBatch();

        renderer->DrawQuad(vec3f(0, 0, 0.5f), Vision::CubeOri::Front, grass.GetID());
        renderer->DrawQuad(vec3f(0, -0.5, 0), Vision::CubeOri::Bottom, grass.GetID());
        renderer->DrawQuad(vec3f(-0.5f, 0, 0), Vision::CubeOri::Left, grass.GetID());
        renderer->DrawQuad(vec3f(0.5f, 0, 0), Vision::CubeOri::Right, grass.GetID());
        renderer->DrawQuad(vec3f(0, 0, -0.5f), Vision::CubeOri::Back, grass.GetID());
        renderer->DrawQuad(vec3f(0, 0.5f, 0), Vision::CubeOri::Top, grass.GetID());
        
        renderer->EndBatch();


        renderer->Flush();
        
        renderer->EndEvents();
    }
    
    grass.~Texture();
    shader->~Shader();
    renderer->Shutdown();
    return 0;
}
