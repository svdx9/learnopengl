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

const char *APP_TITLE = "Introduction to Modern OpenGL - Hello Colored Triangle";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow *gWindow = NULL;

int main()
{
    if (!initOpengl(gWindow, gWindowWidth, gWindowHeight))
    {
        return 1;
    }
    
    return 0;
}
