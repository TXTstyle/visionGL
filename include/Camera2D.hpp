#pragma once
#include <glm/glm.hpp>

#include "Math.hpp"
#include "Renderer2D.hpp"

namespace Vision2D {
    class Camera2D
    {
    private:
        const vec2i &winRes;

        float camSpeed;
        float camZoom;

        glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 0.0f);

        float deltaTime;
        float lastFrame;

        glm::mat4 projMat;
        glm::mat4 viewMat;
    public:
        Camera2D(const vec2i &p_res, float p_zoom, float p_camSpeed);
        ~Camera2D();

        glm::mat4& getProjMat();
        glm::mat4& getViewMat();

        void Controls(Renderer2D &renderer);
    };
}
