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

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error: Vertex shader compilation failed\n"
                  << infoLog << std::endl;
        std::exit(1);
    }

    // create a fragment shader
    auto fragmentShaderSource = loadShader("fragment_shader_1.glsl");
    auto f = fragmentShaderSource.c_str();

    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &f, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error: Fragment shader compilation failed\n"
                  << infoLog << std::endl;
        std::exit(1);
    }

    // create a shader program
    auto shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Error: Shader program linking failed\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // vertex data
    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.0f, 0.5f, 0.0f};

    float vertices1[] = {
        0.25, 0.25, 0.0,
        0.75, 0.25, 0.0,
        0.5, 0.75, 0.0,

        -0.25, 0.25, 0.0,
        -0.75, 0.25, 0.0,
        -0.5, 0.75, 0.0};
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vertex buffer object, this is the buffer that will store our vertex data
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    // tell OpenGL how to interpret the vertex data
    /*
     The function glVertexAttribPointer has quite a few parameters so let's carefully walk through them:

    The first parameter specifies which vertex attribute we want to configure. Remember that we specified the location of the position vertex attribute in the vertex shader with layout (location = 0). This sets the location of the vertex attribute to 0 and since we want to pass data to this vertex attribute, we pass in 0.
    The next argument specifies the size of the vertex attribute. The vertex attribute is a vec3 so it is composed of 3 values.
    The third argument specifies the type of the data which is GL_FLOAT (a vec* in GLSL consists of floating point values).
    The next argument specifies if we want the data to be normalized. If we're inputting integer data types (int, byte) and we've set this to GL_TRUE, the integer data is normalized to 0 (or -1 for signed data) and 1 when converted to float. This is not relevant for us so we'll leave this at GL_FALSE.
    The fifth argument is known as the stride and tells us the space between consecutive vertex attributes. Since the next set of position data is located exactly 3 times the size of a float away we specify that value as the stride. Note that since we know that the array is tightly packed (there is no space between the next vertex attribute value) we could've also specified the stride as 0 to let OpenGL determine the stride (this only works when values are tightly packed). Whenever we have more vertex attributes we have to carefully define the spacing between each vertex attribute but we'll get to see more examples of that later on.
    The last parameter is of type void* and thus requires that weird cast. This is the offset of where the position data begins in the buffer. Since the position data is at the start of the data array this value is just 0. We will explore this parameter in more detail later on

    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // enable the vertex attribute
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0) does not remove or delete the vertex array object (VAO).
    // Instead, it unbinds the currently bound VAO, effectively binding the default VAO.
    // This means that no VAO is currently bound, and subsequent vertex attribute calls will not affect any VAO.
    // Here's a brief explanation of what happens when you call glBindVertexArray(0):
    // If you call glBindVertexArray(0), it unbinds the currently bound VAO.
    // This is useful if you want to ensure that no VAO is bound, which can help prevent accidental modifications to the VAO state.
    // glBindVertexArray(0);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
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

        glDrawArrays(GL_TRIANGLES, 0, 6);
        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
