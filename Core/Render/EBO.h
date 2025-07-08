#ifndef EBO_H
#define EBO_H

#include <span>

#include <glad/glad.h>

class EBO
{
public:
    // EBO reference id
    GLuint m_id{};

    // No default constructor
    EBO() = delete;

    // Constructor for EBO, remember to pass correct GLenum type!
    EBO(std::span<const GLuint> data, GLenum usage = GL_STATIC_DRAW);
    
    // Destructor -> deletes EBO
    ~EBO();

    // Deleted copy and move operations, too lazy to implement them and i don't really need to use them
    EBO(const EBO&) = delete;
    EBO& operator=(const EBO&) = delete;
    EBO(EBO&&) = delete;
    EBO& operator=(EBO&&) = delete;

    // Bind and unbind the EBO
    void Bind();
    void Unbind();
};

#endif // !EBO_H
