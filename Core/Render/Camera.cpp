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
    , m_zoom{ CameraConstants::zoom }
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
    if (direction == forward)
        m_cameraPosition += m_cameraDirection * velocity;
    if (direction == backward)
        m_cameraPosition -= m_cameraDirection * velocity;
    if (direction == left)
        m_cameraPosition -= m_cameraRight * velocity;
    if (direction == right)
        m_cameraPosition += m_cameraRight * velocity;
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
    if (m_zoom < 1.0f)
        m_zoom = 1.0f;
    if (m_zoom > 45.0f)
        m_zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 newDirection{};
    float yawRad{ glm::radians(m_yaw) };
    float pitchRad{ glm::radians(m_pitch) };

    newDirection.x = std::cos(yawRad) * std::cos(pitchRad);
    newDirection.y = std::sin(pitchRad);
    newDirection.z = std::sin(yawRad) * std::cos(pitchRad);
    m_cameraDirection = glm::normalize(newDirection);
    
    m_cameraRight = glm::normalize(glm::cross(m_cameraDirection, m_worldUp));
    m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraDirection));
}
