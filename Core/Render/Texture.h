#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb_image.h>

#include "Shader.h"

class Texture
{
public:
    // Texture id
    GLuint m_id{};
    GLuint m_unit{};

    // No default constructor as we need texture info
    Texture() = delete;

    // Constructor for texture object (Pass path as string literals)
    Texture(const char* filePath, GLenum imageFormat, GLuint unit);

    // Destructor -> deletes texture buffer
    ~Texture();

    // Deleted copy and move operations, too lazy to implement them and i don't really need to use them
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(Texture&&) = delete;

    // Sets the texture unit
    void TexUnit(Shader& shader, const char* uniform, GLuint unit);

    // Binds the texture
    void Bind() const;
};

#endif // !TEXTURE_H
