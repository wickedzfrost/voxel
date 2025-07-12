#include "Camera.h"
#include <cmath>

Camera::Camera(glm::vec3 position, glm::vec3 worldUp)
    : m_cameraPosition{ position }
    , m_worldUp{ worldUp }
    , m_cameraDirection{ CameraConstants::defaultCameraDirection }
    , m_movementSpeed{ CameraConstants::moveSpeed }
    , m_yaw{ CameraConstants::yaw }
    , m_pitch{ CameraConstants::pitch }
    , m_mouseSensitivity{ CameraConstants::sensitivity }
    , m_zoom{ CameraConstants::defaultZoom }
{
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraDirection, m_cameraUp);
}

void Camera::ProcessKeyboard(Direction direction, float deltaTime)
{
    float velocity{ m_movementSpeed * deltaTime };
    constexpr float yLevel{ 0.0f };

    glm::vec3 forwardMovement{ glm::vec3(m_cameraDirection.x, yLevel, m_cameraDirection.z) };
    glm::vec3 rightMovement{ glm::vec3(m_cameraRight.x, yLevel, m_cameraRight.z) };

    if (direction == forward)
        m_cameraPosition += forwardMovement * velocity;
    if (direction == backward)
        m_cameraPosition -= forwardMovement * velocity;
    if (direction == left)
        m_cameraPosition -= rightMovement * velocity;
    if (direction == right)
        m_cameraPosition += rightMovement * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
    m_yaw += xOffset * m_mouseSensitivity;
    m_pitch += yOffset * m_mouseSensitivity;

    if (constrainPitch == GL_TRUE)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::ProcessScrollWheel(float yOffset)
{
    m_zoom -= yOffset;
    if (m_zoom < CameraConstants::minZoom)
        m_zoom = CameraConstants::minZoom;
    if (m_zoom > CameraConstants::maxZoom)
        m_zoom = CameraConstants::maxZoom;
}

void Camera::UpdateCameraVectors()
{
    // Cache yaw and pitch's radians
    float yawRad{ glm::radians(m_yaw) };
    float pitchRad{ glm::radians(m_pitch) };

    glm::vec3 newDirection{};
    newDirection.x = std::cos(yawRad) * std::cos(pitchRad);
    newDirection.y = std::sin(pitchRad);
    newDirection.z = std::sin(yawRad) * std::cos(pitchRad);

    // New camera coords
    m_cameraDirection = glm::normalize(newDirection);
    m_cameraRight = glm::normalize(glm::cross(m_cameraDirection, m_worldUp));
    m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraDirection));
}
