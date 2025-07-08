#include "EBO.h"

EBO::EBO(std::span<const GLuint> data, GLenum usage)
{
    glGenBuffers(1, &m_id);

    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size_bytes(), data.data(), usage);
    Unbind();
}

EBO::~EBO()
{
    glDeleteBuffers(1, &m_id);
}

void EBO::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void EBO::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
