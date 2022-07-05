#include "Renderer2D.hpp"


void GLAPIENTRY Vision2D::errorOccurredGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    if (severity > 0x826b)
    {
        printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
         "Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
        printf("%s\n", message);
        //exit(-1);
    }
}

using namespace Vision2D;

static const size_t MaxQuadCount = 1000;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t MaxIndexCount = MaxQuadCount * 6;

Renderer2D::Renderer2D(const vec2i& p_windowSize, const std::string windowName)
: windowSize(p_windowSize) {
    /* Initialize the library */
    if (!glfwInit())
        exit(-1);
    
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowSize.x, windowSize.y, windowName.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        throw std::runtime_error("GLEW not OK!");

    std::cout << glGetString(GL_VERSION) << std::endl;
}

static RendererData Data;

void Renderer2D::Init() {
    Data.QuadBuffer = new Vertex2D[MaxVertexCount];

    glCreateVertexArrays(1, &Data.QuadVA);
    glBindVertexArray(Data.QuadVA);

    glCreateBuffers(1, &Data.QuadVB);
    glBindBuffer(GL_ARRAY_BUFFER, Data.QuadVB);
    glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexArrayAttrib(Data.QuadVA, 0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, Pos));
    
    glEnableVertexArrayAttrib(Data.QuadVA, 1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, TexCoord));
    
    glEnableVertexArrayAttrib(Data.QuadVA, 2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, Color));
    
    glEnableVertexArrayAttrib(Data.QuadVA, 3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, TexID));

    uint32_t indices[MaxIndexCount];
    uint32_t offset = 0;
    for (int i = 0; i < MaxIndexCount; i += 6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;
        
        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }
    
    glCreateBuffers(1, &Data.QuadIB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Data.QuadIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Create Empty Texture
    glCreateTextures(GL_TEXTURE_2D, 1, &Data.EmptyTexture);
    glBindTexture(GL_TEXTURE_2D, Data.EmptyTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    uint32_t color = 0xffffffff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &color);

    Data.TextureSlots[0] = Data.EmptyTexture;
    for (size_t i = 1; i < 32; i++)
        Data.TextureSlots[i] = 0;
    
}

void Renderer2D::Shutdown() {
    glDeleteVertexArrays(1, &Data.QuadVA);
    glDeleteBuffers(1, &Data.QuadVB);
    glDeleteBuffers(1, &Data.QuadIB);
    
    glDeleteTextures(1, &Data.EmptyTexture);

    delete[] Data.QuadBuffer;
    glfwTerminate();
}

void Renderer2D::StartBatch() {
    Data.QuadBufferPtr = Data.QuadBuffer;
}

void Renderer2D::EndBatch() {
    GLsizeiptr size = (uint8_t*)Data.QuadBufferPtr - (uint8_t*)Data.QuadBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, Data.QuadVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, Data.QuadBuffer);
}

void Renderer2D::Flush() {
    for (uint32_t i = 0; i < Data.TexSlotIndex; i++)
        glBindTextureUnit(i, Data.TextureSlots[i]);

    glBindVertexArray(Data.QuadVA);
    glDrawElements(GL_TRIANGLES, Data.IndexCount, GL_UNSIGNED_INT, nullptr);

    Data.IndexCount = 0;
    Data.TexSlotIndex = 1;
}

void GenVerts(const vec2f p_size, const vec2f p_pos, vec2f *pos) {
    pos[0] = vec2f(p_pos.x - p_size.x, p_pos.y - p_size.y);
    pos[1] = vec2f(p_pos.x - p_size.x, p_pos.y + p_size.y);
    pos[2] = vec2f(p_pos.x + p_size.x, p_pos.y + p_size.y);
    pos[3] = vec2f(p_pos.x + p_size.x, p_pos.y - p_size.y);
}

void Renderer2D::DrawQuad(const vec2f p_pos, const vec2f size, const vec4f color) {
    if (Data.IndexCount >= MaxIndexCount)
    {
        EndBatch();
        Flush();
        StartBatch();
    }

    float textureIndex = 0.0f;
    vec2f pos[4];
    GenVerts(size, p_pos, pos);

    Data.QuadBufferPtr->Pos = pos[0];
    Data.QuadBufferPtr->TexCoord = vec2f(0.0f, 0.0f);
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->TexID = textureIndex;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[1];
    Data.QuadBufferPtr->TexCoord = vec2f(1.0f, 0.0f);
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->TexID = textureIndex;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[2];
    Data.QuadBufferPtr->TexCoord = vec2f(1.0f, 1.0f);
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->TexID = textureIndex;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[3];
    Data.QuadBufferPtr->TexCoord = vec2f(0.0f, 1.0f);
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->TexID = textureIndex;
    Data.QuadBufferPtr++;

    Data.IndexCount += 6;
    
}

void Renderer2D::DrawQuad(const vec2f p_pos, const vec2f size, uint32_t texID) {
    if (Data.IndexCount >= MaxIndexCount || Data.TexSlotIndex > 31)
    {
        EndBatch();
        Flush();
        StartBatch();
    }

    vec4f color = vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    float sizeOffset = 0.5f;
    vec2f pos[4];
    GenVerts(size, p_pos, pos);

    float textureIndex = 0.0f;
    for (uint32_t i = 0; i < Data.TexSlotIndex; i++)
    {
        if (Data.TextureSlots[i] == texID)
        {
            textureIndex = (float)i;
            break;
        }
        
    }
    
    if (textureIndex == 0.0f)
    {
        textureIndex = (float)Data.TexSlotIndex;
        Data.TextureSlots[Data.TexSlotIndex] = texID;
        Data.TexSlotIndex++;
    }
    

    Data.QuadBufferPtr->Pos = pos[0];
    Data.QuadBufferPtr->TexCoord = vec2f(0.0f, 0.0f);
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->TexID = textureIndex;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[1];
    Data.QuadBufferPtr->TexCoord = vec2f(1.0f, 0.0f);
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->TexID = textureIndex;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[2];
    Data.QuadBufferPtr->TexCoord = vec2f(1.0f, 1.0f);
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->TexID = textureIndex;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[3];
    Data.QuadBufferPtr->TexCoord = vec2f(0.0f, 1.0f);
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->TexID = textureIndex;
    Data.QuadBufferPtr++;

    Data.IndexCount += 6;
}

void Renderer2D::Clear(const vec3f color) {
    glClearColor(color.x, color.y, color.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLFWwindow* Renderer2D::getWindow() {
    return window;
}

bool Renderer2D::WindowShouldClose() {
    return (bool)glfwWindowShouldClose(window);
}

void Renderer2D::EndEvents() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void scrollCallback2D(GLFWwindow* window, double xoffset, double yoffset);
void mouseCallback2D(GLFWwindow* window, double xpos, double ypos);

void Renderer2D::InitEnable() {
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(errorOccurredGL, NULL);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT); 

    glfwSetScrollCallback(window, scrollCallback2D);
    glfwSetCursorPosCallback(window, mouseCallback2D);
}
