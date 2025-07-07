#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string_view>

class Shader
{
public:
    // Shader program ID
    GLuint m_id{};

    // No default constructor as we need atleast vertex and fragment shaders
    Shader() = delete;

    // Constructor for shader (Pass path as string literals)
    Shader(const char* vertexPath, const char* fragmentPath);

    // Destructor -> deletes program
    ~Shader();

    // Deleted copy and move operations, too lazy to implement them and i don't really need to use them
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(Shader&&) = delete;

    // Activates the shader program
    void Use();

    /*
        Sets the uniforms for the graphics pipeline

        Try to pass <std::string_view name> by string literals,
        passing strings without null terminator will result in UB
    */
    void setBool(std::string_view name, const bool value) const;
    void setInt(std::string_view name, const int value) const;
    void setFloat(std::string_view name, const float value) const;
    void setMat4(std::string_view name, const glm::mat4& value) const;
};

#endif // !SHADER_H
