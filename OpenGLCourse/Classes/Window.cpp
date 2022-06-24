#include "Window.h"

Window::Window()
{
    width = 1920;
    height = 1080;
    bufferWidth = 0, bufferHeight = 0;
    mainWindow = 0;

    for (auto key : keys)
    {
        key = false;
    }
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;
    bufferWidth = 0, bufferHeight = 0;
    mainWindow = 0;

    for (auto key : keys)
    {
        key = false;
    }
}

int Window::initialize()
{
    if (!glfwInit())
    {
        std::cerr << "GLFW library load failed...\n";
        glfwTerminate();
        return 1;
    }

    // Setup GLFW Window properties

    // OpenGL Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // OpenGL profile without backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // OpenGL profile allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "Main window", NULL, NULL);
    if (!mainWindow)
    {
        std::cerr << "GLFW window creation failed...\n";
        glfwTerminate();
        return 1;
    }

    // OpenGL viewport dimensions
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Handle key and mouse input
    createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Disabling cursor visualization in viewport

    glewExperimental = GLU_TRUE;

    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        std::cerr << "GLEW initialization failed...\n";
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);

    return 0;
}

GLfloat Window::getChangeX()
{
    const GLfloat currentChange = deltaX;
    deltaX = 0;
    return currentChange;
}

GLfloat Window::getChangeY()
{
    const GLfloat currentChange = deltaY;
    deltaY = 0;
    return currentChange;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

void Window::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
            printf("Pressed %c\n", key);
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
            printf("Released %c\n", key);
        }
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->isFirstMouseMovement)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->isFirstMouseMovement = false;
    }

    const float invertY = theWindow->invertMouseY ? -1 : 1;
    theWindow->deltaX = xPos - theWindow->lastX;
    theWindow->deltaY = invertY * (yPos - theWindow->lastY);

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;

    // printf("x: %.3f, y: %.3f \n", theWindow->deltaX, theWindow->deltaY);
}
