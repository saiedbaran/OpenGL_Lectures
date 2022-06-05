#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// windows dimensions
const GLint Width = 1600, Height = 1200;
const float toRadians = 3.1415f / 180.0f;

GLuint VAO, VBO, IBO, shader, uniformModel, uniformProjection;

bool moveRight = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triOffsetStep = 0.5f;

float currentAngle = 0;
float rotationStep = 0.1f;

float PreviousFrameTime = 0;

// Vertex Shader
static const char* vShader = R"(

#version 330                                

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 projection;

out vec4 vCol;

void main()
{
    gl_Position = projection * model * vec4( pos, 1.0);
    vCol = vec4(clamp(pos, 0.0f,1.0f), 1.0f);
}
)";

// Fragment Shader
static const char* fShader = R"(

#version 330

in vec4 vCol;

out vec4 colour;

void main()
{
    colour = vCol;
}
)";

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

void CompileShader()
{
    shader = glCreateProgram();

    if (!shader)
    {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shader, "model");
    uniformProjection = glGetUniformLocation(shader, "projection");
}


void CreateTriangle()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        -1, -1, 0,
        0, -1, 1,
        1, -1, 0,
        0, 1, 0
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3,GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int main()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        printf("GLFW Initialization failed");
        glfwTerminate();

        return 1;
    }

    // Setup GLFW window properties
    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile = No backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(Width, Height, "Test Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();

        return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialization failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();

        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShader();

    glm::mat4 projection = glm::perspective(
        45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.3f, 100.0f
        );

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
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

        glUseProgram(shader);

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

        glBindVertexArray(VAO);

        //** Draw Triangles by vertex
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }


    return 0;
}
