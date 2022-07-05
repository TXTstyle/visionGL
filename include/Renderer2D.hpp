#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
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
        GLFWwindow* window;
        const vec2i& windowSize;
    public:
        Renderer2D(const vec2i& p_windowSize, const std::string windowName);

        static void Init();
        static void Shutdown();

        static void StartBatch();
        static void EndBatch();
        static void Flush();

        void DrawQuad(const vec2f p_pos, const vec2f size, const vec4f color);
        void DrawQuad(const vec2f p_pos, const vec2f size, uint32_t texID);

        void Clear(const vec3f color);
        GLFWwindow* getWindow();
        bool WindowShouldClose();
        void EndEvents();
        void InitEnable();
    };
}