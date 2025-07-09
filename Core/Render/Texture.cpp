#include "Texture.h"
#include <iostream>

Texture::Texture(const char* filePath, GLenum imageFormat, GLuint unit)
    : m_unit{ unit }
{
    stbi_set_flip_vertically_on_load(true);

    // Fetch and load texture data
    int width{};
    int height{};
    int colorChannels{};
    unsigned char* data{ stbi_load(filePath, &width, &height, &colorChannels, 0) };
    if (!data)
    {
        std::cout << "Failed to load texture: " << filePath << '\n';
        throw std::runtime_error{ "Failed to load texture" };
    }

    // Generates texture buffer and bind according to chosen texture unit
    glGenTextures(1, &m_id);
    glActiveTexture(GL_TEXTURE0 + unit);
    Bind();

    // Set wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload data to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Cleanup and unbind
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::TexUnit(Shader& shader, const char* uniform, GLuint unit)
{
    shader.Use();
    glActiveTexture(GL_TEXTURE0 + unit);
    Bind();
    shader.setInt(uniform, unit);
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}