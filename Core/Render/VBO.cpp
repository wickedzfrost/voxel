#include "VBO.h"

VBO::VBO(std::span<const GLfloat> data, GLenum usage)
{
    glGenBuffers(1, &m_id);

    Bind();
    glBufferData(GL_ARRAY_BUFFER, data.size_bytes(), data.data(), usage);
    Unbind();
}

VBO::~VBO()
{
    glDeleteBuffers(1, &m_id);
}

void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VBO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
