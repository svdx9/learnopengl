#include "shader_program.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

ShaderProgram::ShaderProgram(std::filesystem::path shaderDir) : m_shaderDir(shaderDir), m_program{0} {};
ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_program);
};

bool ShaderProgram::loadShaders(std::string vertex_file, std::string fragment_file)
{
    // check for errrors
    int success;
    char infoLog[512];

    // load the vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexShaderSource = fileToString(vertex_file);
    std::string fragmentShaderSource = fileToString(fragment_file);

    const GLchar *v = vertexShaderSource.c_str();
    const GLchar *f = fragmentShaderSource.c_str();

    glShaderSource(vs, 1, &v, NULL); // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glShaderSource.xhtml
    glShaderSource(fs, 1, &f, NULL);

    glCompileShader(vs);
    if (hasCompileErrors(vs, ShaderProgram::ShaderStep::VERTEX))
    {
        return false;
    }
    glCompileShader(fs);
    if (hasCompileErrors(fs, ShaderProgram::ShaderStep::FRAGMENT))
    {
        return false;
    }

    m_program = glCreateProgram();
    if (m_program == 0)
    {
        std::cerr << "Unable to create shader program!" << std::endl;
        return false;
    }
    glAttachShader(m_program, vs);
    glAttachShader(m_program, fs);
    glLinkProgram(m_program);
    if (hasCompileErrors(m_program, ShaderProgram::ShaderStep::LINK))
    {
        return false;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return true;
};

bool ShaderProgram::hasCompileErrors(GLuint shader, const ShaderProgram::ShaderStep step)
{
    // check for errrors
    int success = GL_FALSE;

    if (step == ShaderProgram::ShaderStep::LINK)
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            GLint error_len = 0;
            glGetShaderiv(m_program, GL_INFO_LOG_LENGTH, &error_len);

            std::string errorlog(error_len, ' ');
            glGetShaderInfoLog(shader, error_len, NULL, &errorlog[0]);
            std::cerr << "Error: program link failed: "
                      << errorlog << std::endl;
            return true;
        }
    }
    else
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
        {
            GLint error_len = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_len);

            std::string errorlog(error_len, ' ');
            glGetShaderInfoLog(shader, error_len, NULL, &errorlog[0]);
            std::cerr << "Error: shader compilation failed: "
                      << errorlog << std::endl;
            return true;
        }
    }
    return false;
};

void ShaderProgram::use()
{
    if (m_program > 0)
    {
        glUseProgram(m_program);
    }
};

GLint ShaderProgram::getProgram()
{
    return m_program;
}

std::string ShaderProgram::fileToString(const std::string &filePath)
{
    std::filesystem::path file = filePath;
    std::filesystem::path fullPath = m_shaderDir / file;

    std::ifstream shaderFile(fullPath);
    if (!shaderFile.is_open())
    {
        throw std::runtime_error("Failed to open shader file: " + fullPath.string());
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    return shaderStream.str();
};
