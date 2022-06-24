#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Camera
{
public:
    Camera();
    Camera(glm::vec3 startPosition, glm::vec3 startUp = glm::vec3(0, 1, 0), GLfloat startYaw = 0,
           GLfloat startPitch = -90, GLfloat startMoveSpeed = 1, GLfloat startTurnRate = 1);
    ~Camera() {}

    void keyControl(bool* keys, GLfloat deltaTime = 1);
    void mouseControl(GLfloat deltaX, GLfloat deltaY);
    
    glm::mat4 calculateViewMatrix(bool hasTarget = false, glm::vec3 targetPosition = glm::vec3(0));

private:
    void update();

private:
    glm::vec3 position;
    glm::vec3 forward, right, up, worldUp;

    GLfloat yaw, pitch;

    GLfloat moveSpeed;
    GLfloat turnRate;
};
