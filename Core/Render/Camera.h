#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace CameraConstants
{
    inline constexpr float moveSpeed{ 0.1f };
}

class Camera
{
public:
    enum Direction
    {
        forward,
        backward,
        left,
        right,
    };

    glm::vec3 m_cameraPosition{};
    glm::vec3 m_cameraDirection{};
    glm::vec3 m_cameraUp{};
    glm::vec3 m_cameraRight{};
    glm::vec3 m_worldUp{};
    float m_movementSpeed{};

    Camera(glm::vec3 position, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(Direction direction, float deltaTime);

private:
    static constexpr float m_sensitivity{ 0.1f };
    glm::mat4 m_view{};

    void UpdateCameraVectors();
};

#endif // !CAMERA_H
