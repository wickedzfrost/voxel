#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <initializer_list>
#include <glm/gtc/type_ptr.hpp>

// === Helper Functions === //
namespace
{
    std::string readShaderFile(const char* filePath)
    {
        std::string shaderCode;
        std::ifstream shaderFile;

        // Ensures ifstream objects can throw exceptions
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            shaderFile.open(filePath);

            std::stringstream shaderStream;

            shaderStream << shaderFile.rdbuf();

            shaderFile.close();

            shaderCode = shaderStream.str();
        }
        catch ([[maybe_unused]] const std::ifstream::failure& e)
        {
            std::cout << "Failed to read shader file: " << filePath << '\n';
            return {};
        }

        return shaderCode;
    }

    void checkCompileErrors(GLuint shader, std::string_view type)
    {
        int success{};
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << type << " compilation error: " << infoLog << '\n';
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << type << " linking error: " << infoLog << '\n';
            }
        }
    }

    GLuint compileShader(const char* shaderCode, GLenum shaderType)
    {
        GLuint shader{ glCreateShader(shaderType) };
        glShaderSource(shader, 1, &shaderCode, nullptr);
        glCompileShader(shader);

        checkCompileErrors(shader, "SHADER");
        return shader;
    }

    GLuint createProgram(std::initializer_list<GLuint> shaderList)
    {
        GLuint programID{ glCreateProgram() };

        // Attach valid shaders
        for (GLuint shader : shaderList)
            if (shader != 0)
                glAttachShader(programID, shader);

        glLinkProgram(programID);

        checkCompileErrors(programID, "PROGRAM");

        // Delete shaders
        for (GLuint shader : shaderList)
            if (shader != 0)
                glDeleteShader(shader);

        return programID;
    }
}

// === Shader Class === //
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // Reads shader file and saves them into a string
    const std::string vertexSource{ readShaderFile(vertexPath) };
    const std::string fragmentSource{ readShaderFile(fragmentPath) };
    
    const char* vertexCode{ vertexSource.c_str() };
    const char* fragmentCode{ fragmentSource.c_str() };

    GLuint vertexID{ compileShader(vertexCode, GL_VERTEX_SHADER) };
    GLuint fragmentID{ compileShader(fragmentCode, GL_FRAGMENT_SHADER) };

    // Shaders are deleted after this point
    GLuint programID{ createProgram({ vertexID, fragmentID }) };

    std::cout << "Shader linked: \n\t - " << vertexPath << "\n\t - " << fragmentPath << "\n\n";
    m_id = programID;
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

void Shader::Use()
{
    glUseProgram(m_id);
}

void Shader::SetBool(std::string_view name, const bool value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.data()), static_cast<int>(value));
}

void Shader::SetInt(std::string_view name, const int value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.data()), value);
}

void Shader::SetFloat(std::string_view name, const float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name.data()), value);
}

void Shader::SetVec2(std::string_view name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(m_id, name.data()), 1, glm::value_ptr(value));
}

void Shader::SetVec3(std::string_view name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(m_id, name.data()), 1, glm::value_ptr(value));
}

void Shader::SetVec4(std::string_view name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(m_id, name.data()), 1, glm::value_ptr(value));
}

void Shader::SetMat2(std::string_view name, const glm::mat2& value) const
{
    glUniformMatrix2fv(glGetUniformLocation(m_id, name.data()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat3(std::string_view name, const glm::mat3& value) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_id, name.data()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4(std::string_view name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.data()), 1, GL_FALSE, glm::value_ptr(value));
}