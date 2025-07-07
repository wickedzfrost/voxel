#ifndef VBO_H
#define VBO_H

#include <span>

#include <glad/glad.h>

class VBO
{
public:
    // VBO reference id
    GLuint m_id{};

    // No default constructor
    VBO() = delete;

    // Constructor for VBO, remember to pass correct GLenum type!
    VBO(std::span<const GLfloat> data, GLenum usage = GL_STATIC_DRAW);

    // Destructor -> deletes VBO
    ~VBO();

    // Deleted copy and move operations, too lazy to implement them and i don't really need to use them
    VBO(const VBO&) = delete;
    VBO& operator=(const VBO&) = delete;
    VBO(VBO&&) = delete;
    VBO& operator=(VBO&&) = delete;

    // Bind and unbind the VBO
    void Bind();
    void Unbind();
};

#endif // !VBO_H
