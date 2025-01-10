#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <filesystem>

#include "utility.h"
#include "shader_program.hpp"

// Global Variables
const char *APP_TITLE = "Introduction to Modern OpenGL - Hello Colored Triangle";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow *gWindow = NULL;

int main(void)
{

    if (!initOpengl(gWindow, gWindowWidth, gWindowHeight))
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

    std::filesystem::path dir = getEnvVar("SHADERS_DIR");
    ShaderProgram s = ShaderProgram(dir);
    if (!s.loadShaders("01_shader.vs", "01_shader.fs"))
    {
        return 1;
    };

    // render loop
    while (!glfwWindowShouldClose(gWindow))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glClearColor.xhtml
        glClear(GL_COLOR_BUFFER_BIT);         // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glClear.xhtml

        double time = glfwGetTime();
        float green_val = ((float)sin(time) / 2.0f) + 0.5f;
        int vertex_col_location = glGetUniformLocation(s.getProgram(), "ourColor");
        s.use();
        glUniform4f(vertex_col_location, 0.0f, green_val, 0.0f, 1.0f);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glfwSwapBuffers(gWindow);
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return 0;
}
