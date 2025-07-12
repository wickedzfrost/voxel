#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace CameraConstants
{
    inline constexpr glm::vec3 defaultCameraDirection{ 0.0f, 0.0f, -1.0f };
    inline constexpr float moveSpeed{ 2.5f };
    inline constexpr float yaw{ -90.0f };
    inline constexpr float pitch{ 0.0f };
    inline constexpr float sensitivity{ 0.05f };
    inline constexpr float defaultZoom{ 45.0f };
    inline constexpr float minZoom{ 1.0f };
    inline constexpr float maxZoom{ 55.0f };
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

    // Camera attributes
    glm::vec3 m_cameraPosition{};
    glm::vec3 m_cameraDirection{};
    glm::vec3 m_cameraUp{};
    glm::vec3 m_cameraRight{};
    glm::vec3 m_worldUp{};
    
    // Euler angles
    float m_yaw{};
    float m_pitch{};

    // Camera properties
    float m_movementSpeed{};
    float m_mouseSensitivity{};
    float m_zoom{};

    // No default constructor, supply with atleast camera position
    Camera() = delete;

    // Constructor using glm::vec3
    Camera(glm::vec3 position, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f));

    ~Camera() = default;

    // Deleted copy and move operations, too lazy to implement them and i don't really need to use them
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(Camera&&) = delete;

    glm::mat4 GetViewMatrix();

    // Input processing functions
    void ProcessKeyboard(Direction direction, float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = GL_TRUE);
    void ProcessScrollWheel(float yOffset);

private:

    void UpdateCameraVectors();
};

#endif // !CAMERA_H
