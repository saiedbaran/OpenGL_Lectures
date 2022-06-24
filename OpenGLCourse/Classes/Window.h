#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);

    int initialize();

    GLint getBufferWidth() { return bufferWidth; }
    GLint getBufferHeight() { return bufferHeight; }
    bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

    bool* getkeys() {return keys;}

    GLfloat getChangeX();
    GLfloat getChangeY();
    
    void swapBuffers() { glfwSwapBuffers(mainWindow); }

    bool invertMouseY = true;
    
    ~Window();
private:
    GLFWwindow* mainWindow;
    GLint width, height;
    GLint bufferWidth, bufferHeight;

    bool keys[1024];

    GLfloat lastX, lastY, deltaX, deltaY;
    bool isFirstMouseMovement = true;

    void createCallbacks();
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};
