#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Classes/Mesh.h"
#include "Classes/Shader.h"
#include "Classes/Window.h"

// windows dimensions
const GLint Width = 1600, Height = 1200;
const float toRadians = 3.1415f / 180.0f;

Window mainWindow;
std::vector<Mesh> meshList;
std::vector<Shader> shaderList;

// path to shaders
static const char* vShader = "C:/Users/ssafi/Documents/Repositories/OpenGL_Lectures/OpenGLCourse/Shaders/vShader.vert";
static const char* fShader = "C:/Users/ssafi/Documents/Repositories/OpenGL_Lectures/OpenGLCourse/Shaders/fShader.frag";

bool moveRight = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triOffsetStep = 0.5f;

float currentAngle = 0;
float rotationStep = 0.1f;

float PreviousFrameTime = 0;

void CreateObject()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(*obj1);
}

void CreateShader()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main()
{
    mainWindow.initialize();

    CreateObject();
    CreateShader();


    GLuint uniformModel, uniformProjection;
    glm::mat4 projection = glm::perspective(
        45.0f,
        (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(),
        0.3f, 100.0f
    );

    // Loop until window closed
    while (!mainWindow.getShouldClose())
    {
        // Get + Handle user input events
        glfwPollEvents();

        float deltaTime = 0;
        deltaTime = glfwGetTime() - PreviousFrameTime;
        PreviousFrameTime = glfwGetTime();

        if (moveRight)
        {
            triOffset += triOffsetStep * deltaTime;
        }
        else
        {
            triOffset -= triOffsetStep * deltaTime;
        }

        if (std::abs(triOffset) >= triMaxOffset) { moveRight = !moveRight; }

        currentAngle += rotationStep;
        if (currentAngle >= 360) { currentAngle = 0; }

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        
        // identity matrix 4x4
        glm::mat4 model = glm::mat4(1);
        // translate model by triOffset in X direction
        model = glm::translate(model, glm::vec3(0, 0, -3));
        model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(0.4, 0.4, 1));

        // create Uniform matrix 4x4
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        // create Uniform matrix 4x4 for projection
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projection));

        meshList[0].RenderMesh();

        // translate model by triOffset in X direction
        model = glm::translate(model, glm::vec3(0, 1, -3));
        model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(0.4, 0.4, 1));

        // create Uniform matrix 4x4
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        // create Uniform matrix 4x4 for projection
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projection));
        

        glUseProgram(0);

        mainWindow.swapBuffers();
    }


    return 0;
}
