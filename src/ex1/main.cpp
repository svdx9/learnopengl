#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <filesystem>

// Global Variables
const char *APP_TITLE = "Introduction to Modern OpenGL - Hello Colored Triangle";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow *gWindow = NULL;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
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

bool initOpengl()
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

    gWindow = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (gWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
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
    framebuffer_size_callback(gWindow, gWindowWidth, gWindowHeight);
    return true;
}

int main(void)
{

    if (!initOpengl())
    {
        std::cerr << "glfw initialisation failed" << std::endl;
        return -1;
    }

    // 1. setup an array of vertices with position and color for a triangle

    GLfloat vertices[] = {
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

    // 2. Set up buffer on the GPU
    GLuint vao, vbo;

    glGenBuffers(1, &vbo);                                                     // Generate an enmpty vertex buffer  on the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo);                                        // Bind the vertex buffer we are working with
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy the data from cpu to gpu

    // https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object
    // A Vertex Array Object (VAO) is an OpenGL Object that stores all of the state needed to supply vertex data.
    // It stores the format of the vertex data as well as the Buffer Objects (see below) providing the vertex data arrays.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // tell OpenGL how to interpret the vertex data
    // The function glVertexAttribPointer has quite a few parameters so let's carefully walk through them:

    // The first parameter specifies which vertex attribute we want to configure. Remember that we specified the location of the position vertex
    // attribute in the vertex shader with layout (location = 0). This sets the location of the vertex attribute to 0 and since we want to pass
    // data to this vertex attribute, we pass in 0.
    // The next argument specifies the size of the vertex attribute. The vertex attribute is a vec3 so it is composed of 3 values.
    // The third argument specifies the type of the data which is GL_FLOAT (a vec* in GLSL consists of floating point values).
    // The next argument specifies if we want the data to be normalized. If we're inputting integer data types (int, byte) and we've set this to GL_TRUE,
    // the integer data is normalized to 0 (or -1 for signed data) and 1 when converted to float. This is not relevant for us so we'll leave this at GL_FALSE.
    // The fifth argument is known as the stride and tells us the space between consecutive vertex attributes. Since the next set of position data is
    // located exactly 3 times the size of a float away we specify that value as the stride. Note that since we know that the array is tightly packed
    // (there is no space between the next vertex attribute value) we could've also specified the stride as 0 to let OpenGL determine the stride (this
    // only works when values are tightly packed). Whenever we have more vertex attributes we have to carefully define the spacing between each vertex
    // attribute but we'll get to see more examples of that later on.
    // The last parameter is of type void* and thus requires that weird cast. This is the offset of where the position data begins in the buffer.
    //  Since the position data is at the start of the data array this value is just 0. We will explore this parameter in more detail later on
    // the position attribute, identified as 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
    // color attribute, identified as 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(sizeof(GLfloat) * 3));
    // enable the vertex attribute
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // 3. Create vertex shader
    std::string vertexShaderSource = loadShader("vertex_shader_1.glsl");
    const GLchar *v = vertexShaderSource.c_str();
    GLuint vs = glCreateShader(GL_VERTEX_SHADER); // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShader.xhtml
    glShaderSource(vs, 1, &v, NULL);              // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glShaderSource.xhtml
    glCompileShader(vs);

    // 4. Create fragment shader
    std::string fragmentShaderSource = loadShader("fragment_shader_1.glsl");
    const GLchar *f = fragmentShaderSource.c_str();
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER); // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShader.xhtml
    glShaderSource(fs, 1, &f, NULL);                // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glShaderSource.xhtml
    glCompileShader(fs);

    // check for errrors
    int success;
    char infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cout << "Error: Vertex shader compilation failed\n"
                  << infoLog << std::endl;
        std::exit(1);
    }
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cout << "Error: Fragment shader compilation failed\n"
                  << infoLog << std::endl;
        std::exit(1);
    }

    // 5. Create shader program and link shaders to program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs); // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glAttachShader.xhtml
    glAttachShader(shaderProgram, fs); // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glAttachShader.xhtml
    glLinkProgram(shaderProgram);      // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glLinkProgram.xhtml

    // Check for linker errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Error: Shader program linking failed\n"
                  << infoLog << std::endl;
    }

    // Clean up shaders, do not need them anymore since they are linked to a shader program
    glDeleteShader(vs);
    glDeleteShader(fs);

    // render loop
    while (!glfwWindowShouldClose(gWindow))
    {
        glfwPollEvents();
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glfwSwapBuffers(gWindow);
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
