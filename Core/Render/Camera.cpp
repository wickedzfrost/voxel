#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp)
    : m_cameraPosition{ position }
    , m_worldUp{ worldUp }
    , m_cameraDirection{ glm::vec3(0.0f, 0.0f, -1.0f) }
    , m_movementSpeed{ CameraConstants::moveSpeed }
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

void Camera::UpdateCameraVectors()
{
    m_cameraRight = glm::normalize(glm::cross(m_cameraDirection, m_worldUp));
    m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraDirection));
}
