#pragma once

// Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace dog {

    class Camera {
    public: 
        Camera(const Camera&) = delete;
        Camera(Camera&&) = delete;
        Camera& operator=(const Camera&) = delete;
        Camera& operator=(Camera&&) = delete;   

    private:
        glm::vec3 m_eye = glm::vec3(0.0f, 10.0f, 0.0f);
        glm::vec3 m_center = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 m_viewMatrix = glm::lookAt(m_eye, m_center, m_up);
};

}

