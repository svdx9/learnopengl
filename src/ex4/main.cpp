#include <iostream>

#include "glm/glm.hpp"
#include "glm/vec4.hpp"
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp>  // glm::pi
#include <glm/gtc/matrix_transform.hpp>  // For glm::rotate
#include "glm/gtc/type_ptr.hpp"

#include "utility.h"
#include "stb_image.h"
#include "shader_program.hpp"

// int main()
// {
//     glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
//     glm::mat4 trans = glm::mat4(1.0f);
//     trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
//     vec = trans * vec;
//     std::cout << vec.x << vec.y << vec.z << std::endl;
// }

const char *APP_TITLE = "Introduction to Modern OpenGL - Hello Colored Triangle";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow *gWindow = NULL;

std::filesystem::path shaderDir = getEnvVar("SHADERS_DIR");
std::filesystem::path assetsDir = getEnvVar("ASSETS_DIR");

int main()
{

    if (!initOpengl(gWindow, gWindowWidth, gWindowHeight))
    {
        std::cerr << "glfw initialisation failed" << std::endl;
        return -1;
    }

    std::filesystem::path shaderDir = getEnvVar("SHADERS_DIR");
    ShaderProgram s(shaderDir);
    if (!s.loadShaders("01_shader.vs", "01_shader.fs"))
    {
        return 1;
    }

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // postion attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(sizeof(GLfloat) * 6));
    glEnableVertexAttribArray(2);

    // glBindTexture(GL_TEXTURE_2D, texture);

    int w, h, nrChannels;
    unsigned char *data;
    std::string t;
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // glActiveTexture(GL_TEXTURE1);
    // set the texture wrapping parameters

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    t = assetsDir / "container.jpg";
    data = stbi_load(t.c_str(), &w, &h, &nrChannels, 0);
    if (data == NULL)
    {
        std::cout << "Failed to load texture" << std::endl;
        return 1;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    t = assetsDir / "awesomeface.png";
    data = stbi_load(t.c_str(), &w, &h, &nrChannels, 0);
    if (data == NULL)
    {
        std::cout << "Failed to load texture face" << std::endl;
        return 1;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    s.use();
    glUniform1i(glGetUniformLocation(s.getProgram(), "texture1"), 0);
    glUniform1i(glGetUniformLocation(s.getProgram(), "texture2"), 1);

    // create a model matrix, the model matrix consists of tranlations, scaling and/or rotations
    // we'd like to apply to transform all object's vertices to the global worlds space. Lwt's transfom our
    // plane a bit by rotating it on the x-axis so it looks like it's laying on the floor.
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // create a view matrix:
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // define the projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // send the model, view and projection matrices to the shaders
    while (!glfwWindowShouldClose(gWindow))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        s.use();
        int modelLoc = glGetUniformLocation(s.getProgram(), "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(s.getProgram(), "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLoc = glGetUniformLocation(s.getProgram(), "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(gWindow);
        glfwPollEvents();
    }

    return 0;
}
