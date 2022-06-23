#include "Camera.h"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>


Camera::Camera()
{
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed,
               GLfloat startTurnRate)
{
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    forward = glm::vec3(0, 0, -1);

    moveSpeed = startMoveSpeed;
    turnRate = startTurnRate;

    update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
    if (keys[GLFW_KEY_W])
    {
        position += forward * moveSpeed * deltaTime;
    }
    if (keys[GLFW_KEY_S])
    {
        position -= forward * moveSpeed * deltaTime;
    }
    if (keys[GLFW_KEY_D])
    {
        position += right * moveSpeed * deltaTime;
    }
    if (keys[GLFW_KEY_A])
    {
        position -= right * moveSpeed * deltaTime;
    }
}

glm::mat4 Camera::calculateViewMatrix(bool hasTarget, glm::vec3 targetPosition)
{
    glm::mat4 viewMatrix;
    glm::vec3 targetPos = position + forward;

    if (hasTarget)
    {
        targetPos = targetPosition;
    }

    return glm::lookAt(position, targetPos, up);
}

void Camera::update()
{
    forward.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    forward.y = glm::sin(glm::radians(pitch));
    forward.z = glm::sin(glm::radians(yaw)) * glm::sin(glm::radians(pitch));
    forward = glm::normalize(forward);

    right = glm::normalize(glm::cross(forward, worldUp));
    up = glm::normalize(glm::cross(right, forward));
}
