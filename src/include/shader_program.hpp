#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <string>
#include <filesystem>
#include <glad/glad.h>
class ShaderProgram
{
public:
    ShaderProgram(std::filesystem::path shaderDir);
    ~ShaderProgram();
    enum ShaderStep
    {
        VERTEX,
        FRAGMENT,
        LINK,
    };
    bool loadShaders(std::string vs, std::string fs);
    void use();

private:
    std::string fileToString(const std::string &filePath);
    bool hasCompileErrors(GLuint shader, const ShaderStep type);
    // vars
    GLuint m_program;
    std::filesystem::path m_shaderDir;
};

#endif // SHADER_PROGRAM_HPP
