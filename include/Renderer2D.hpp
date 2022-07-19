#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <array>

#include "Math.hpp"

namespace Vision2D{

    void GLAPIENTRY errorOccurredGL(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar* message, const void* userParam);


    struct RendererData
    {
        uint32_t QuadVA = 0;
        uint32_t QuadVB = 0;
        uint32_t QuadIB = 0;

        uint32_t EmptyTexture = 0;
        uint32_t EmptyTexSlot = 0;

        uint32_t IndexCount = 0;

        Vertex2D* QuadBuffer = nullptr;
        Vertex2D* QuadBufferPtr = nullptr;

        std::array<uint32_t, 32> TextureSlots;
        uint32_t TexSlotIndex = 1;
    };

    class Renderer2D {
    private:
        Renderer2D();
    public:
        static GLFWwindow* window;
        static vec2i windowSize;

        static void Init();
        static void Shutdown();

        static void StartBatch();
        static void EndBatch();
        static void Flush();

        static void DrawQuad(const glm::vec2 p_pos, const glm::vec2 size, const vec4f color);
        static void DrawQuad(const glm::vec2 p_pos, const glm::vec2 size, std::string texName);

        static void Clear(const glm::vec3 color);
        static bool WindowShouldClose();
        static void EndEvents();
        static void InitEnable(const vec2i& p_windowSize, const std::string windowName);
    };
}