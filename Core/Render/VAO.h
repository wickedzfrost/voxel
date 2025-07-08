#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
public:
    // VAO reference id
    GLuint m_id{};

    // Constructor
    VAO();

    // Destructor -> deletes VAO
    ~VAO();

    // Deleted copy and move operations, too lazy to implement them and i don't really need to use them
    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;
    VAO(VAO&&) = delete;
    VAO& operator=(VAO&&) = delete;

    // Sets the vertex attrib pointer
    void LinkVBO(VBO& vbo, GLuint layout, GLint size, GLenum type, GLsizei stride, std::size_t offset);

    // Bind and unbind the VAO
    void Bind();
    void Unbind();
};

#endif // !VAO_H
