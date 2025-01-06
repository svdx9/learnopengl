#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <filesystem>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// Function to get an environment variable
std::string getEnvVar(const std::string &key)
{
    const char *val = std::getenv(key.c_str());
    if (val == nullptr)
    {
        throw std::runtime_error("Environment variable not found: " + key);
    }
    return std::string(val);
}

// Function to load a shader from a file
std::string loadShader(const std::string &filePath)
{
    std::filesystem::path dir = getEnvVar("SHADERS_DIR");
    std::filesystem::path file = filePath;
    std::filesystem::path fullPath = dir / file;

    std::ifstream shaderFile(fullPath);
    if (!shaderFile.is_open())
    {
        throw std::runtime_error("Failed to open shader file: " + filePath);
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    return shaderStream.str();
}

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initialise glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int success;
    char infoLog[512];

    // create a vertex shader
    auto vertexShaderSource = loadShader("vertex_shader_1.glsl");
    auto v = vertexShaderSource.c_str();
    auto fragmentShaderSource1 = loadShader("fragment_shader_1.glsl");
    auto f1 = fragmentShaderSource1.c_str();
    auto fragmentShaderSource2 = loadShader("fragment_shader_2.glsl");
    auto f2 = fragmentShaderSource2.c_str();

    auto shaderProgram1 = glCreateProgram();
    auto shaderProgram2 = glCreateProgram();

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &v, NULL);
    glShaderSource(fragmentShader1, 1, &f1, NULL);
    glShaderSource(fragmentShader2, 1, &f2, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader1);
    glCompileShader(fragmentShader2);

    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glAttachShader(shaderProgram2, fragmentShader2);

    glLinkProgram(shaderProgram1);
    glLinkProgram(shaderProgram2);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);

    float vertices1[] = {
        0.25, 0.25, 0.0,
        0.75, 0.25, 0.0,
        0.5, 0.75, 0.0};

    float vertices2[] = {
        -0.25, 0.25, 0.0,
        -0.75, 0.25, 0.0,
        -0.5, 0.75, 0.0};

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // vertex buffer object, this is the buffer that will store our vertex data
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        // rendering commands here
        // ...
        // clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // copy our vertex data into the buffer
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram1);
    glfwTerminate();
    return 0;
}
