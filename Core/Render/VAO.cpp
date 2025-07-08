#include "VAO.h"

VAO::VAO()
{
    glGenVertexArrays(1, &m_id);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &m_id);
}

void VAO::LinkAttrib(VBO& vbo, GLuint layout, GLint size, GLenum type, GLsizei stride, std::size_t offset) 
{
    Bind();
    vbo.Bind();

    glVertexAttribPointer(layout, size, type, GL_FALSE, stride, reinterpret_cast<const void*>(offset));
    glEnableVertexAttribArray(layout);

    vbo.Unbind();
    Unbind();
}

void VAO::Bind()
{
    glBindVertexArray(m_id);
}

void VAO::Unbind()
{
    glBindVertexArray(0);
}
