#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>

#include "Math.hpp"

namespace Vision{

    void GLAPIENTRY errorOccurredGL(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar* message, const void* userParam);

    enum CubeOri {
        Front,
        Bottom,
        Left,
        Right,
        Back,
        Top
    };

    struct RendererData
    {
        uint32_t QuadVA = 0;
        uint32_t QuadVB = 0;
        uint32_t QuadIB = 0;

        uint32_t EmptyTexture = 0;
        uint32_t EmptyTexSlot = 0;

        uint32_t IndexCount = 0;

        Vertex* QuadBuffer = nullptr;
        Vertex* QuadBufferPtr = nullptr;

        std::array<uint32_t, 32> TextureSlots;
        uint32_t TexSlotIndex = 1;
    };

    class Renderer {
    private:
        GLFWwindow* window;
        const vec2i& windowSize;
    public:
        Renderer(const vec2i& p_windowSize, const std::string windowName);

        static void Init();
        static void Shutdown();

        static void StartBatch();
        static void EndBatch();
        static void Flush();

        void DrawQuad(const vec3f p_pos, const CubeOri ori, const vec4f color);
        void DrawQuad(const vec3f p_pos, const CubeOri ori, uint32_t texID);

        void Clear(const vec3f color);
        GLFWwindow* getWindow();
        bool WindowShouldClose();
        void EndEvents();
        void InitEnable();
    };
}