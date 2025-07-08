#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
public:
    GLuint m_id{};

    VAO();

    ~VAO();

    void LinkVBO(VBO& vbo, GLuint layout, GLint size, GLenum type, GLsizei stride, std::size_t offset);
    void Bind();
    void Unbind();
};

#endif // !VAO_H
