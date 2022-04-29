#include <stdio.h>
#include  <GL/glew.h>
#include <GLFW/glfw3.h>

// windows dimensions
const GLint Width = 1920, Height = 1080;

int main()
{
    // Initialise GLFW
    if(!glfwInit())
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

    GLFWwindow* mainWindow = glfwCreateWindow(Width,Height, "Test Window", NULL, NULL);
    if(!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();

        return 1;
    }
    
    return 0;
}