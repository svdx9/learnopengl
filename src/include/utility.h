#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <filesystem>

std::string getEnvVar(const std::string &key)
{
    const char *val = std::getenv(key.c_str());
    if (val == nullptr)
    {
        throw std::runtime_error("Environment variable not found: " + key);
    }
    return std::string(val);
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
bool initOpengl(GLFWwindow *&gWindow, int width, int height)
{
    if (!glfwInit())
    {
        // An error occured
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3

    gWindow = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (gWindow == NULL)
    {
        std::cout << "noooooooooo Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(gWindow);
    // initialise glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return false;
    }
    // Set the required callback functions
    glfwSetKeyCallback(gWindow, glfw_onKey);
    glfwSetFramebufferSizeCallback(gWindow, framebuffer_size_callback);

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

    // Define the viewport dimensions
    framebuffer_size_callback(gWindow, width, height);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    return true;
}
